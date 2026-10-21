/*
 * A071944  Triangle read by rows giving numbers of paths in a lattice
 *          satisfying certain conditions.
 * https://oeis.org/A071944
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) is the (n‑th) entry of the triangle T(n,k) read row‑by‑row,
 *            where T(0,0)=1 and for k>0 T(n,k)=T(n,k-1)+T(n-1,k)+T(n-1,k-3),
 *            with T(n,k)=0 for k<0
 * Range:     u8 n <= 32, u16 n <= 74, u32 n <= 206, u64 n <= 628,
 *            u128 n <= 2207, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Find row r such that r(r+1)/2 <= n < (r+1)(r+2)/2.  */
    idx_t r = 0;
    while (((unsigned __int128)(r + 1) * (r + 2)) / 2 <= n)
        ++r;

    /* Column index within that row.  */
    unsigned __int128 pref = (unsigned __int128)r * (r + 1) / 2;
    idx_t col = n - (idx_t)pref;

    seq_t *prev = NULL;
    seq_t *curr = NULL;

    for (idx_t i = 0; i <= r; ++i) {
#ifdef __cplusplus
        curr = new seq_t[i + 1];
#else
        curr = (seq_t*)malloc((i + 1) * sizeof(seq_t));
#endif
        if (!curr) return (seq_t)0;   /* allocation failure – safe default */

        if (i == 0) {
            curr[0] = (seq_t)1;
        } else {
            /* First element of the row comes from the previous row. */
            curr[0] = prev[0];

            for (idx_t k = 1; k <= i; ++k) {
                seq_t val = curr[k - 1];          /* T(n,k-1)   */
                if (k < i)                       /* T(n-1,k)   */
                    val += prev[k];
                if (k >= 3)                       /* T(n-1,k-3) */
                    val += prev[k - 3];
                curr[k] = val;
            }
        }

        /* Recycle the previous row's storage. */
        if (prev) {
#ifdef __cplusplus
            delete[] prev;
#else
            free(prev);
#endif
        }
        prev = curr;
    }

    seq_t result = prev[col];
#ifdef __cplusplus
    delete[] prev;
#else
    free(prev);
#endif
    return result;
}
