/*
 * A106295  Period of the Lucas 4-step sequence A073817 mod n.
 * https://oeis.org/A106295
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm( a(p_i^e_i) for all prime powers dividing n ) where
 *            a(2^k) = 5 * 2^(k-1) (k > 0) and for odd primes: if a(p) !=
 *            a(p^2) then a(p^k) = p^(k-1) * a(p) else a(p^k) = a(p)
 * Range:     u8 n <= 4, u16 n <= 42, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   period_mod, period_prime_power
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Assumes seq_t and idx_t are defined by the build.
    Uses 64‑bit arithmetic for indices and a simple Euclidean gcd for seq_t. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/period_mod.h"
#include "../utils/period_prime_power.h"

/* Euclidean gcd for seq_t (works for unsigned types of any size). */
static inline seq_t gcd_seq(seq_t a, seq_t b)
{
    while (b) {
        seq_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* n >= 1 */
    if (n == 0) return 0;    /* undefined, but return 0 */

    uint64_t rem = (uint64_t)n;
    seq_t result = 1;       /* lcm accumulator */

    /* factorization by trial division */
    for (uint64_t p = 2; p * p <= rem; ++p) {
        if (rem % p == 0) {
            unsigned exp = 0;
            while (rem % p == 0) {
                rem /= p;
                ++exp;
            }
            seq_t per_p = period_prime_power((uint32_t)p, exp);
            seq_t g = gcd_seq(result, per_p);
            result = (result / g) * per_p;
        }
    }
    if (rem > 1) {          /* remaining prime factor */
        seq_t per_p = period_prime_power((uint32_t)rem, 1);
        seq_t g = gcd_seq(result, per_p);
        result = (result / g) * per_p;
    }

    return result;
}
