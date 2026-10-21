/*
 * A226192  Expansion of phi(x^2) * psi(-x) in powers of x where phi(), psi()
 *          are Ramanujan theta functions.
 * https://oeis.org/A226192
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = b(8*n + 1) where b is multiplicative with b(2^e) = 0^e,
 *            b(p^e) = (-1)^(e*(p+1)/8) for p ≡ 3 (mod 4), b(p^e) = (e+1) *
 *            i^(e*(p-1)/4) for p ≡ 1 (mod 4). Since 8*n+1 is odd, only odd
 *            primes occur. The product of the complex factors always results
 *            in ±1, so the final value is an integer
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* m = 8*n + 1 */
    uint64_t m = (uint64_t)n * 8ULL + 1ULL;
    seq_t result = 1;            /* multiplicative product */
    uint8_t exp_mod4 = 0;        /* cumulative exponent of i modulo 4 */
    bool zero_flag = false;

    uint64_t temp = m;

    /* trial division by odd primes */
    for (uint64_t p = 3; (__int128)p * p <= temp; p += 2) {
        if (temp % p != 0) continue;

        uint64_t e = 0;
        while (temp % p == 0) {
            temp /= p;
            e++;
        }

        if (p % 4 == 3) {
            /* zero when exponent is odd */
            if (e & 1) {
                zero_flag = true;
                break;
            }
            /* sign: (-1)^(e*(p+1)/8) */
            __int128 k = (__int128)e * (p + 1ULL) / 8ULL;
            if (k & 1) {
                result = -result;
            }
        } else { /* p % 4 == 1 */
            /* integer part */
            result *= (seq_t)(e + 1);

            /* accumulate exponent of i modulo 4 */
            __int128 k = (__int128)e * (p - 1ULL) / 4ULL;
            exp_mod4 = (uint8_t)((exp_mod4 + (k & 3ULL)) & 3ULL);
        }
    }

    if (!zero_flag && temp > 1) {   /* remaining prime factor */
        uint64_t p = temp;
        uint64_t e = 1;
        if (p % 4 == 3) {
            /* e is odd -> zero */
            zero_flag = true;
        } else { /* p % 4 == 1 */
            result *= (seq_t)(e + 1);

            __int128 k = (__int128)e * (p - 1ULL) / 4ULL;
            exp_mod4 = (uint8_t)((exp_mod4 + (k & 3ULL)) & 3ULL);
        }
    }

    if (zero_flag) return (seq_t)0;

    /* apply the aggregated sign from the i-exponents */
    if (exp_mod4 == 2) {
        result = -result;
    } else if (exp_mod4 == 1 || exp_mod4 == 3) {
        /* This case should not occur for m = 8*n + 1;
           if it does, the value would be non‑real. */
        result = 0;
    }

    return result;
}
