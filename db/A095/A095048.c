/*
 * A095048  Number of distinct digits needed to write all positive divisors of
 *          n in decimal representation.
 * https://oeis.org/A095048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of distinct decimal digits appearing in all
 *            positive divisors of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_bits, factor_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: factor the integer, enumerate all divisors,
   accumulate the decimal digits seen in a 10‑bit mask, then count those bits. */
#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_uint64.h"
#include "../utils/count_bits.h"

/* Recursive enumeration of all divisors of n given its prime factors.
   The current product is 'cur', idx points to the current prime factor. */
static void enumerate_divisors(uint64_t cur, int idx,
                               const uint64_t *pr,
                               const int *e,
                               int cnt,
                               uint16_t *mask)
{
    if (idx == cnt)
    {
        uint64_t d = cur;
        /* Record the digits of this divisor. */
        if (d == 0) /* shouldn't happen for positive divisors */
            return;
        while (d > 0)
        {
            *mask |= 1u << (d % 10);
            d /= 10;
        }
        return;
    }
    uint64_t v = cur;
    for (int i = 0; i <= e[idx]; ++i)
    {
        enumerate_divisors(v, idx + 1, pr, e, cnt, mask);
        if (i < e[idx])
            v *= pr[idx];
    }
}

seq_t a(idx_t n)
{
    if (n == 0) /* safety – OEIS offset starts at 1 */
    {
        return (seq_t)0;
    }

    uint64_t fac[20];
    int exp[20];
    int cnt; /* number of distinct prime factors */

    /* Factor n.  The helper writes the factors into fac/exp
       and stores the count in cnt. */
    factor_uint64(n, fac, exp, &cnt);

    uint16_t mask = 0;
    enumerate_divisors(1, 0, fac, exp, cnt, &mask);

    return (seq_t)count_bits(mask);
}
