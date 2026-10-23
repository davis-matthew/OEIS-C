/*
 * A046738  Period of Fibonacci 3-step sequence A000073 mod n.
 * https://oeis.org/A046738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm( a(p^e) for each prime power p^e dividing n ), with
 *            a(p^e) = period of the tribonacci sequence modulo p^e
 * Range:     u8 n <= 14, u16 n <= 194, u32 n <= 9579, u64 n <= 9579,
 *            u128 n <= 9579, big any n
 * Helpers:   gcd, trib_period
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Tribonacci: 0,0,1 and T_k = T_{k-1}+T_{k-2}+T_{k-3})             */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/trib_period.h"

/* Fast exponentiation for uint64_t (no overflow for m <= 10000). */
static uint64_t pow_uint_u64(uint64_t base, uint32_t exp)
{
    uint64_t result = 1;
    while (exp)
    {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* Main function: returns a(n) for the tribonacci Pisano period. */
seq_t a(idx_t n)
{
    if (n <= 1)            /* a(1) = 1 by convention */
        return (seq_t)1;

    /* Factorise n (n ≤ 10000, so trial division is fast). */
    uint64_t primes[16];
    uint32_t exps[16];
    uint32_t pcnt = 0;
    uint64_t tmp = (uint64_t)n;

    /* factor 2 */
    if ((tmp & 1ULL) == 0)
    {
        uint32_t e = 0;
        while ((tmp & 1ULL) == 0) { tmp >>= 1; e++; }
        primes[pcnt] = 2;
        exps[pcnt++] = e;
    }

    /* odd factors */
    for (uint64_t d = 3; d * d <= tmp; d += 2)
    {
        if (tmp % d == 0)
        {
            uint32_t e = 0;
            while (tmp % d == 0) { tmp /= d; e++; }
            primes[pcnt] = d;
            exps[pcnt++] = e;
        }
    }

    /* remaining prime */
    if (tmp > 1)
    {
        primes[pcnt] = tmp;
        exps[pcnt] = 1;
        pcnt++;
    }

    /* lcm of the periods for each prime power */
    seq_t lcm_val = (seq_t)1;

    for (uint32_t i = 0; i < pcnt; ++i)
    {
        uint64_t p      = primes[i];
        uint32_t e      = exps[i];
        uint64_t mod    = pow_uint_u64(p, e);
        seq_t period_pe = trib_period(mod);

        seq_t g  = gcd(lcm_val, period_pe);
        lcm_val = (lcm_val / g) * period_pe;
    }

    return lcm_val;
}
