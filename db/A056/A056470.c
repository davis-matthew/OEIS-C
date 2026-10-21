/*
 * A056470  Number of palindromic structures using a maximum of five different
 *          symbols.
 * https://oeis.org/A056470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=1..5} S(ceil(n/2),k) where S(m,k) are Stirling
 *            numbers of the second kind. This counts the number of
 *            palindromic structures on at most five distinct symbols up to
 *            permutation of the symbols
 * Range:     u8 n <= 12, u16 n <= 18, u32 n <= 32, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>      /* provided by the build; seq_t and idx_t are already typedef'd */

seq_t a(idx_t n)
{
    /* ceil(n/2) */
    idx_t m = (n + 1) / 2;
    if (m == 0) return (seq_t)0;   /* for safety when n < 1 */

    /* Stirling rows: store only the current and previous row */
    seq_t prev[6] = {0, 0, 0, 0, 0, 0};
    seq_t cur[6]  = {0, 0, 0, 0, 0, 0};

    prev[0] = (seq_t)1;   /* S(0,0) = 1 */

    for (idx_t i = 1; i <= m; ++i) {
        cur[0] = (seq_t)0;
        idx_t kmax = i < 5 ? i : 5;
        for (idx_t k = 1; k <= kmax; ++k) {
            /* S(i,k) = k * S(i-1,k) + S(i-1,k-1) */
            cur[k] = prev[k] * (seq_t)k + prev[k - 1];
        }
        /* Remaining columns > i are zero (unchanged) */
        for (idx_t k = kmax + 1; k <= 5; ++k) {
            cur[k] = (seq_t)0;
        }
        /* Prepare for next iteration */
        for (idx_t k = 0; k <= 5; ++k) {
            prev[k] = cur[k];
        }
    }

    /* Sum the values for k = 1..5 */
    seq_t total = (seq_t)0;
    for (idx_t k = 1; k <= 5; ++k) {
        total += prev[k];
    }
    return total;
}
