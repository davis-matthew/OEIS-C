/*
 * A228441  Expansion of Sum_{k>0} -(-x)^k / (1 + x^k).
 * https://oeis.org/A228441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) = number of divisors of n minus 4 times the number of
 *            divisors of n of the form 4*k+2. The function implements the
 *            multiplicative form: a(2^e) = e - 3 (for e > 0), a(p^e) = e + 1
 *            for odd primes p. Multiplicativity gives a(n) = Π a(p_i^e_i)
 * Range:     i8 n <= 5000, i16 n <= 5000, i32 n <= 5000, i64 n <= 5000,
 *            i128 n <= 5000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    if (n == 0) return 0; /* guard against invalid call */
    if (n == 1) return 1; /* a(1) = 1 */

    uint64_t N = (uint64_t)n;
    /* Max distinct prime factors for 64‑bit numbers is <64, so buffer is safe. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    factor_u64(N, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t val;
        if (primes[i] == 2) {
            val = (seq_t)exps[i] - (seq_t)3;
        } else {
            val = (seq_t)exps[i] + (seq_t)1;
        }
        result *= val;
    }
    return result;
}
