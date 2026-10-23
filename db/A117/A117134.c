/*
 * A117134  Greatest k such that n^k divides (n^2)!.
 * https://oeis.org/A117134
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = max{k | n^k divides (n^2)!} Computed by factoring n and
 *            using Legendre's formula for each prime factor
 * Range:     u8 n <= 17, u16 n <= 629, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 2) return 0;                  /* sequence starts at n = 2 */

    idx_t temp = n;
    seq_t result = 0;
    bool first = true;

    const idx_t n2 = n * n;                /* n^2 fits in idx_t for n <= 10000 */

    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }

            /* exponent of prime p in (n^2)! using Legendre's formula */
            seq_t exp = 0;
            idx_t power = p;
            while (power <= n2) {
                exp += (seq_t)(n2 / power);
                if (power > n2 / p) break;   /* avoid overflow of power *= p */
                power *= p;
            }

            seq_t allowed = exp / (seq_t)e;
            if (first || allowed < result) {
                result = allowed;
                first = false;
            }
        }
    }

    /* if temp > 1, it is a prime factor > sqrt(original n) */
    if (temp > 1) {
        idx_t p = temp;
        seq_t exp = 0;
        idx_t power = p;
        while (power <= n2) {
            exp += (seq_t)(n2 / power);
            if (power > n2 / p) break;
            power *= p;
        }

        if (first || exp < result) {
            result = exp;
            first = false;
        }
    }

    return result;
}
