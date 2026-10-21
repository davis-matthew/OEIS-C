/*
 * A095407  Total number of decimal digits of all distinct prime factors of n.
 * https://oeis.org/A095407
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = Σ_{p|n, p distinct} (decimal digit length of p)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_count, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses factor_u64 to obtain the distinct prime factors of n, then
   sums digit_count(p) for each.  For n = 1 the value is 0. */

#include <stdint.h>
#include <stddef.h>

#include "../utils/factor_u64.h"   /* for factor_u64 */
#include "../utils/digit_count.h" /* for digit_count */

seq_t a(idx_t n)
{
    /* a(1) = 0 */
    if (n <= 1)
        return (seq_t)0;

    /* factor_u64 expects a uint64_t argument */
    uint64_t n64 = (uint64_t)n;

    /* Prime factors and their exponents.  64 is plenty for 64‑bit n. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64(n64, primes, exps, &cnt);

    seq_t total = 0;
    for (size_t i = 0; i < cnt; ++i)
        total += (seq_t)digit_count(primes[i]);

    return total;
}
