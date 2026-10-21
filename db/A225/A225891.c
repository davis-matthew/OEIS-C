/*
 * A225891  Numbers obtained by rotating left the indices in the prime
 *          signature of n.
 * https://oeis.org/A225891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   rotate left the exponents in the prime signature of n * a(n) =
 *            ∏_{i=1}^{k} p_i^{e_{i+1}} (indices modulo k) * where n =
 *            ∏_{i=1}^{k} p_i^{e_i} with primes p_i ascending. * For n=1 (no
 *            prime factors) a(1)=1
 * Range:     u8 n <= 51, u16 n <= 271, u32 n <= 2367, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    /* Handle the trivial case */
    if (n == 1u) return 1;

    /* Prime factorization: at most 64 distinct prime factors for 64‑bit n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    if (cnt == 0) return 1; /* Should not happen for n>1 */

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        /* Exponent rotated left: new exponent at position i comes from
         * the next index (with wrap around). */
        uint64_t exp = exps[(i + 1) % cnt];
        seq_t base = (seq_t)primes[i];
        /* Multiply by base^exp */
        while (exp) {
            result *= base;
            --exp;
        }
    }
    return result;
}
