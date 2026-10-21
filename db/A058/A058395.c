/*
 * A058395  Square array read by antidiagonals. Based on triangular numbers
 *          (A000217) with each term being the sum of 2 consecutive terms in
 *          the previous row.
 * https://oeis.org/A058395
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Let T(n,k) be defined by T[0,k] = (k%2==0) ? ((k/2+2)*(k/2+1))/2
 *            : 0 T[n,0] = 1 for n>0 T[n,k] = T[n-1,k] + T[n-1,k-1] for n>0,
 *            k>0 Then the sequence a(n) is read by antidiagonals of T: find s
 *            with s*(s+1)/2 ≤ n < (s+1)*(s+2)/2, let i = n - s*(s+1)/2, j = s
 *            - i, a(n) = T[i][j]. This produces the...
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

static seq_t T[201][201];
static bool ready = false;

seq_t a(idx_t n)
{
    /* Prepare the array on the first call */
    if (!ready) {
        const idx_t MAX = 200;          /* sufficient for n ≤ 10000 */
        /* Base row: k = 0..MAX */
        for (idx_t k = 0; k <= MAX; ++k) {
            if ((k & 1) == 0) {
                idx_t m = k >> 1;          /* m = k/2 */
                T[0][k] = ((seq_t)(m + 2) * (seq_t)(m + 1)) / 2;
            } else {
                T[0][k] = 0;
            }
        }
        /* Remaining rows */
        for (idx_t nrow = 1; nrow <= MAX; ++nrow) {
            T[nrow][0] = (seq_t)1;          /* T[n,0] = 1 */
            for (idx_t k = 1; k <= MAX; ++k) {
                T[nrow][k] = T[nrow-1][k] + T[nrow-1][k-1];
            }
        }
        ready = true;
    }

    /* Find the diagonal index s such that s*(s+1)/2 ≤ n < (s+1)*(s+2)/2 */
    idx_t s = 0;
    while ((s + 1) * (s + 2) / 2 <= n) {
        ++s;
    }

    /* Off‑diagonal offset */
    idx_t d = n - s * (s + 1) / 2;
    idx_t i = d;
    idx_t j = s - d;   /* i + j = s */

    /* Bounds are guaranteed to be within MAX (≈200) for n ≤ 10000 */
    return T[i][j];
}
