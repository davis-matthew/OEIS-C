/*
 * A166653  Totally multiplicative sequence with a(p) = 4p-1 for prime p.
 * https://oeis.org/A166653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = product over prime power factors of n: (4p‑1)^e
 * Range:     u8 n <= 7, u16 n <= 63, u32 n <= 4095, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, seq_mul, seq_one, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_one.h"

seq_t a(idx_t n)
{
    /* The OEIS offset is 1; values for n < 1 are undefined. */
    if (n == 0)
        return 0;  /* guard against illegal input */

    /* Factor n using the provided routine. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = seq_one();

    for (size_t i = 0; i < cnt; ++i)
    {
        /* Compute (4p - 1) for the current prime. */
        seq_t base = (seq_t)4 * (seq_t)primes[i] - (seq_t)1;
        /* Raise to the corresponding exponent. */
        seq_t term = seq_pow(base, (idx_t)exps[i]);
        /* Accumulate the product. */
        result = seq_mul(result, term);
    }

    return result;
}
