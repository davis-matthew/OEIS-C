/*
 * A071549  a(n) = (7n)!/n!^7.
 * https://oeis.org/A071549
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7n)! / (n!)^7 * = Π_{j=1}^{6} C((7-j)n, n) * Recurrence:
 *            a(0)=1 * a(n)=a(n-1)*∏_{k=1}^7 (7(n-1)+k)/n^7
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 1, u64 n <= 3, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    seq_t res = 1;
    for (idx_t k = 1; k <= n; ++k) {
        /* product of 7 consecutive integers: 7*(k-1)+1 … 7*(k-1)+7 */
        seq_t prod = 1;
        seq_t base = 7 * ((seq_t)k - 1);
        for (int j = 1; j <= 7; ++j) {
            prod *= (base + j);
        }
        res *= prod;

        /* denominator k^7 */
        seq_t denom = (seq_t)k;
        for (int j = 1; j < 7; ++j) {
            denom *= (seq_t)k;
        }
        res /= denom;   /* division is exact */
    }
    return res;
}
