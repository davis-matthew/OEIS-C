/*
 * A124573  Triangle read by rows where the n-th row is the first row of M^n,
 *          with M the (n+1)-by-(n+1) matrix with (3,1,3,1,3,1,...) on its
 *          main diagonal and (1,3,1,3,1,3,...) on its superdiagonal.
 * https://oeis.org/A124573
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = entry at (0, j) of M^r, where M is the (r+1)-by-(r+1) *
 *            bidiagonal matrix with M[i][i] = (i even ? 3 : 1) and M[i][i+1]
 *            = (i even ? * 1 : 3). The pair (r, j) is obtained by unfolding
 *            the triangle, i.e. * r is the smallest integer with (r+1)(r+2)/2
 *            > n, and j = n - r(r+1)/2. * * The impleme...
 * Range:     u8 n <= 16, u16 n <= 46, u32 n <= 174, u64 n <= 602,
 *            u128 n <= 2230, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* determine row r and column j */
    idx_t r = 0;
    while ( ((seq_t)((unsigned __int128)(r + 1) * (r + 2) / 2)) <= n )
        ++r;

    seq_t start = (seq_t)((unsigned __int128)r * (r + 1) / 2);
    idx_t j = (idx_t)(n - start);

    /* allocate array for the first row of M^r */
    seq_t *cur = (seq_t*)malloc((size_t)(r + 1) * sizeof(seq_t));
    if (!cur) return 0;                /* should not happen for normal inputs */

    for (idx_t k = 1; k <= r; ++k) cur[k] = 0;  /* zero initialize */
    cur[0] = 1;                              /* initial row for M^0 */

    /* iterate r times, each time multiplying by M */
    for (idx_t step = 1; step <= r; ++step) {
        for (idx_t k = step; k > 0; --k) {
            /* contribution from staying at position k */
            seq_t stay = cur[k] * ((k % 2 == 0) ? (seq_t)3 : (seq_t)1);
            /* contribution from moving from position k-1 */
            seq_t jump = cur[k - 1] *
                         (((k - 1) % 2 == 0) ? (seq_t)1 : (seq_t)3);
            cur[k] = stay + jump;
        }
        /* first position always uses diagonal entry 3 */
        cur[0] *= 3;
    }

    seq_t result = cur[j];
    free(cur);
    return result;
}
