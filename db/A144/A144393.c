/*
 * A144393  Triangle read by rows (n >= 0, 0 <= k <= n): row n gives the
 *          coefficients in the expansion of x^n + n*x^(n - 1) + n*x + 1.
 * https://oeis.org/A144393
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle read by rows; the sequence is obtained by flattening the
 * coefficients of the polynomial x^r + r*x^(r-1) + r*x + 1.
 * For a given global index n:
 *   r = floor((sqrt(8n+1)-1)/2)          // row number
 *   k = n - r(r+1)/2                    // position in the row (0 ≤ k ≤ r)
 * The coefficient is:
 *   r = 0  -> 2
 *   r = 1  -> 2
 *   r = 2  -> (k==0||k==2) ? 1 : 4
 *   r ≥ 3  -> (k==0||k==r)       ? 1
 *            : (k==1||k==r-1)   ? r
 *            : 0
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* find the row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
    idx_t r = 0;
    while (1) {
        __int128 tri = (__int128)r * (r + 1) / 2;
        if (tri > n) break;
        ++r;
    }
    --r;                       // r is now the correct row

    idx_t start = (__int128)r * (r + 1) / 2;
    idx_t k = n - start;        // position inside the row

    if (r == 0) return (seq_t)2;        /* row 0: single coefficient 2 */

    if (r == 1) return (seq_t)2;        /* row 1: coefficients 2,2 */

    if (r == 2) {                      /* row 2: 1,4,1 */
        if (k == 0 || k == 2) return (seq_t)1;
        if (k == 1) return (seq_t)4;
        return 0; // unreachable
    }

    /* r >= 3 */
    if (k == 0 || k == r) return (seq_t)1;           /* leading & constant */
    if (k == 1 || k == r - 1) return (seq_t)r;       /* x^1 and x^(r-1) */
    return 0;                                          /* all other spots */
}
