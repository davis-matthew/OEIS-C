/*
 * A075500  Stirling2 triangle with scaled diagonals (powers of 5).
 * https://oeis.org/A075500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 57, u64 n <= 157,
 *            u128 n <= 470, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Recurrence (lower‐triangular form):
        a(1,1) = 1
        a(r,1) = 5 * a(r-1,1)                      for r>1
        a(r,m) = 5*m*a(r-1,m) + a(r-1,m-1)          for 2≤m≤r
    The sequence a(n) is the flattened row‑major array
    a(1,1), a(2,1),a(2,2), a(3,1),a(3,2),a(3,3), …   (offset 1)
*/

#include <stdlib.h>
#include <stdint.h>
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* offset 1 */

    /* Find row r such that T(r-1) < n ≤ T(r) where T(r)=r*(r+1)/2 */
    idx_t r = 1;
    idx_t tri = 1;                     /* T(1) */
    while (tri < n) {
        ++r;
        tri += r;
    }
    idx_t prev_tri = tri - r;          /* T(r-1) */
    idx_t m = n - prev_tri;            /* column (1‑based) */

#ifdef __cplusplus
    /* C++ path – use std::vector to ensure proper construction of big ints */
    std::vector<seq_t> prev_row;
    for (idx_t cur = 1; cur <= r; ++cur) {
        std::vector<seq_t> cur_row(cur + 1);   /* default‑constructed zeros */

        if (cur == 1) {
            cur_row[1] = 1;                  /* a(1,1) */
        } else {
            cur_row[1] = prev_row[1] * (seq_t)(5 * 1);
            for (idx_t j = 2; j <= cur; ++j) {
                seq_t term1 = (j <= cur - 1) ? prev_row[j] : 0;
                seq_t term2 = prev_row[j - 1];
                cur_row[j] = term1 * (seq_t)(5 * j) + term2;
            }
        }
        prev_row.swap(cur_row);              /* cur becomes new previous row */
    }
    return prev_row[m];

#else
    /* C path – allocate and free arrays manually (seq_t is a plain integer type) */
    seq_t *prev_row = NULL;
    for (idx_t cur = 1; cur <= r; ++cur) {
        seq_t *cur_row = (seq_t *)malloc((size_t)(cur + 1) * sizeof(seq_t));
        if (!cur_row) abort();              /* allocation failure */
        for (idx_t k = 0; k <= cur; ++k)   /* zero initialise */
            cur_row[k] = 0;

        if (cur == 1) {
            cur_row[1] = 1;
        } else {
            cur_row[1] = prev_row[1] * (seq_t)(5 * 1);
            for (idx_t j = 2; j <= cur; ++j) {
                seq_t term1 = (j <= cur - 1) ? prev_row[j] : 0;
                seq_t term2 = prev_row[j - 1];
                cur_row[j] = term1 * (seq_t)(5 * j) + term2;
            }
        }
        free(prev_row);
        prev_row = cur_row;
    }
    seq_t result = prev_row[m];
    free(prev_row);
    return result;
#endif
}
