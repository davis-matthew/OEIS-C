/*
 * A055491  Smallest square divisible by n divided by largest square which
 *          divides n.
 * https://oeis.org/A055491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, mult
 * Formula:   a(n) = ∏_{primes p|n} p^{2*(e_p mod 2)} where e_p is the
 *            exponent of p in the prime factorisation of n. Equivalently,
 *            multiply by p² for each prime whose exponent in n is odd
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 1;
    idx_t x = n;
    idx_t cnt;

    /* handle factor 2 separately */
    if ((x & 1u) == 0) {
        cnt = 0;
        while ((x & 1u) == 0) {
            x >>= 1;
            ++cnt;
        }
        if (cnt & 1u) {
            result *= (seq_t)4;           /* 2² */
        }
    }

    /* odd primes */
    for (idx_t p = 3; p <= x / p; p += 2) {
        if (x % p == 0) {
            cnt = 0;
            while (x % p == 0) {
                x /= p;
                ++cnt;
            }
            if (cnt & 1u) {
                seq_t square = (seq_t)p * (seq_t)p;
                result *= square;
            }
        }
    }

    /* remaining prime factor, if any */
    if (x > 1) {
        seq_t square = (seq_t)x * (seq_t)x;
        result *= square;
    }

    return result;
}
