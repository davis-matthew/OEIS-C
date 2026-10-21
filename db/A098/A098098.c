/*
 * A098098  a(n) = sigma(6*n+5)/6.
 * https://oeis.org/A098098
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sigma(6*n+5)/6
 * Range:     u8 n <= 34, u16 n <= 9999, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    /* compute m = 6*n + 5 (all in idx_t) */
    idx_t m = n * 6ULL + 5ULL;
    /* sigma_of returns the sum of divisors of m */
    seq_t sum = sigma_of(m);
    /* division by 6 is exact for all m ≡ 5 (mod 6) */
    return sum / 6;
}
