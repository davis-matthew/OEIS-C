/*
 * A063962  Number of distinct prime divisors of n that are <= sqrt(n).
 * https://oeis.org/A063962
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = count of distinct prime divisors p of n with p <= sqrt(n)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _floor_isqrt_u64, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This uses trial division factors from the utils library. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/_floor_isqrt_u64.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* For n = 1 the result is 0. */
    if (n < 2)
        return 0;

    /* Floor of the square root of n */
    idx_t sqrt_n = (idx_t)_floor_isqrt_u64(n);

    /* Factor n.  The factor_u64 routine fills arrays with distinct primes
       and their exponents (which we ignore).  The number of distinct
       primes found is returned in cnt. */
    uint64_t pr[32];            /* distinct primes */
    uint64_t ex[32];            /* exponents (unused) */
    size_t   cnt;

    factor_u64((uint64_t)n, pr, ex, &cnt);

    /* Count how many of the distinct primes are <= sqrt(n). */
    seq_t count = 0;
    for (size_t i = 0; i < cnt; ++i)
        if (pr[i] <= sqrt_n)
            ++count;

    return count;
}
