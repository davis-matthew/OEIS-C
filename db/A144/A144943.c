/*
 * A144943  a(n) = number of divisors of n^3 (excluding 1 and n^3).
 * https://oeis.org/A144943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors of n^3 excluding 1 and n^3 =
 *            product_{p^e || n} (3*e + 1) - 2 for n ≥ 2, a(1) = 0 See OEIS
 *            A144943
 * Range:     u8 n <= 359, u16 n <= 1001, u32 n <= 1001, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    seq_t result = 1;
    uint64_t k = (uint64_t)n;
    uint64_t d;

    for (uint64_t p = 2; p * p <= k; ++p) {
        if (k % p == 0) {
            d = 0;
            while (k % p == 0) {
                k /= p;
                ++d;
            }
            result *= (seq_t)(3 * d + 1);
        }
    }

    if (k > 1) {  /* remaining prime factor with exponent 1 */
        result *= (seq_t)(3 * 1 + 1);
    }

    result -= 2;   /* exclude 1 and n^3 itself */
    return result;
}
