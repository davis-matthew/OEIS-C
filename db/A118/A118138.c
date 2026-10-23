/*
 * A118138  Sum of factorials of prime factors, with multiplicity.
 * https://oeis.org/A118138
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{p^e‖n} e * p! (sum of factorials of prime factors,
 *            with multiplicity)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 12, u64 n <= 22, u128 n <= 28,
 *            big any n
 * Helpers:   factor_u64, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factor_u64.h"
#include "../utils/factorial.h"

seq_t a(idx_t n) {
    /* OEIS offset is 2: a(2) is the first term. For n<2 the value is undefined. */
    if (n < 2)
        return 0;

    /* Factor n.  The maximum number of distinct prime factors for a 64‑bit integer
       is well below 64, so we can allocate fixed-size buffers on the stack. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i) {
        /* factorial(p) returns a seq_t; multiply by the exponent (converted to seq_t) */
        seq_t factp = factorial((idx_t)primes[i]);
        sum += factp * (seq_t)exps[i];
    }

    return sum;
}
