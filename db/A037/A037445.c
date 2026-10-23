/*
 * A037445  Number of infinitary divisors (or i-divisors) of n.
 * https://oeis.org/A037445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy, mult
 * Formula:   a(n) = product_{p^e || n} 2^popcount(e) (multiplicative)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/pow2.h"

/* popcount for 64‑bit integers */
static unsigned int popcnt_uint64(uint64_t x)
{
    unsigned int c = 0;
    while (x)
    {
        x &= x - 1;   /* drop lowest set bit */
        ++c;
    }
    return c;
}

seq_t a(idx_t n)
{
    if (n <= 1)          /* a(1) = 1, a(0) not used but harmless */
        return (seq_t)1;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64(n, primes, exps, &cnt);

    seq_t res = 1;
    for (size_t i = 0; i < cnt; ++i)
    {
        unsigned int pc = popcnt_uint64(exps[i]);
        res *= pow2((idx_t)pc);
    }
    return res;
}
