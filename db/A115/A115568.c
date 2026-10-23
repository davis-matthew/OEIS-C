/*
 * A115568  Maximum Fibonacci exponent in prime factorization of n, or 1 if
 *          there is no Fibonacci exponent.
 * https://oeis.org/A115568
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = maximum exponent in the prime factorization of n that is
 *            a Fibonacci number; if none, return 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

static const unsigned fibs[] = {1U, 1U, 2U, 3U, 5U, 8U, 13U, 21U, 34U, 55U, 89U, 144U};
static const size_t fibcnt = sizeof(fibs) / sizeof(fibs[0]);

static bool is_fibon(unsigned e)
{
    for (size_t i = 0; i < fibcnt; ++i)
    {
        if (fibs[i] == e)
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    unsigned max_f = 1;
    for (size_t i = 0; i < cnt; ++i)
    {
        unsigned e = (unsigned)exps[i];
        if (e > max_f && is_fibon(e))
            max_f = e;
    }
    return (seq_t)max_f;
}
