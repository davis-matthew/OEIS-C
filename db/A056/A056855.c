/*
 * A056855  a(n) = (Product k) * (Sum 1/k), where both the product and the sum
 *          are over those positive integers k, where k <= n and gcd(k,n) = 1.
 * https://oeis.org/A056855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (Product k) * (Sum 1/k), where both the product and the
 *            sum * are over all positive integers k <= n with gcd(k,n) = 1
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 22, u128 n <= 36,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t prod = 1;
    for (idx_t k = 1; k <= n; ++k) {
        if (gcd((seq_t)k, (seq_t)n) == 1) {
            prod *= (seq_t)k;
        }
    }

    seq_t res = 0;
    for (idx_t k = 1; k <= n; ++k) {
        if (gcd((seq_t)k, (seq_t)n) == 1) {
            res += prod / (seq_t)k;
        }
    }
    return res;
}
