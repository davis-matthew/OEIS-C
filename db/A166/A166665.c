/*
 * A166665  Totally multiplicative sequence with a(p) = 7p+1 for prime p.
 * https://oeis.org/A166665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e‖n} (7p+1)^e (multiplicative with a(p^e) =
 *            (7p+1)^e)
 * Range:     u8 n <= 5, u16 n <= 23, u32 n <= 511, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* Undefined for n < 1, but safe return */
    if (n == 1) return (seq_t)1;

    /* Temporary storage for prime factors of n.
       64 primes are enough for 64-bit n. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t base = (seq_t)(7 * primes[i] + 1);
        /* Compute base^exps[i] by repeated multiplication. */
        seq_t pow = (seq_t)1;
        for (size_t j = 0; j < exps[i]; ++j)
            pow *= base;
        result *= pow;
    }
    return result;
}
