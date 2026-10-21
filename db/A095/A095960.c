/*
 * A095960  Number of divisors of n that are less than the squarefree kernel
 *          of n.
 * https://oeis.org/A095960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ d | n : d < rad(n) }
 * Range:     u8 n <= 2048, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _gen_div, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of divisors of n that are less than the squarefree kernel of n. */

#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/_gen_div.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;        /* a(1) = 0, n>=1 is guaranteed */

    /* Factor n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Compute rad(n) = product of distinct prime factors */
    seq_t rad = 1;
    for (size_t i = 0; i < cnt; i++)
        rad *= (seq_t)primes[i];

    /* Count divisors d of n with d < rad(n) */
    seq_t count = 0;
    _gen_div(0, (seq_t)1, primes, exps, cnt, rad, &count);

    return count;
}
