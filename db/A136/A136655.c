/*
 * A136655  Product of odd divisors of n.
 * https://oeis.org/A136655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = product of odd divisors of n a(n) = product of all
 *            divisors of the odd part of n
 * Range:     u8 n <= 20, u16 n <= 44, u32 n <= 224, u64 n <= 944,
 *            u128 n <= 3464, big any n
 * Helpers:   floor_sqrt, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_pow.h"   /* seq_pow(seq_t base, idx_t exp) */
#include "../utils/seq_mul.h"   /* seq_mul(seq_t a, seq_t b) */
#include "../utils/floor_sqrt.h" /* floor_sqrt(seq_t x) */

static seq_t odd_part(idx_t n)
{
    while ((n & 1ULL) == 0)
        n >>= 1;
    return (seq_t)n;
}

/* compute a(n) */
seq_t a(idx_t n)
{
    /* odd part of n */
    seq_t m = odd_part(n);
    if (m == (seq_t)1)
        return (seq_t)1;

    /* factor m and compute tau(m) */
    uint64_t m_val = (uint64_t)m;
    uint64_t tmp = m_val;
    uint64_t tau = 1;
    for (uint64_t p = 3; p * p <= tmp; p += 2)
    {
        if (tmp % p == 0)
        {
            uint64_t exp = 0;
            while (tmp % p == 0)
            {
                tmp /= p;
                exp++;
            }
            tau *= (exp + 1);
        }
    }
    if (tmp > 1)          /* remaining prime factor */
        tau *= 2;         /* exponent 1 -> (1+1) */

    /* magic: product of all divisors of m = m^(tau/2) */
    seq_t res;
    if ((tau & 1ULL) == 0) /* tau even: exponent integer */
    {
        res = seq_pow(m, tau / 2);
    }
    else /* tau odd: m must be a perfect square */
    {
        uint64_t sqrt_val = (uint64_t)floor_sqrt(m);
        seq_t sqrt_m = (seq_t)sqrt_val;
        res = seq_pow(m, (tau - 1) / 2);
        res = seq_mul(res, sqrt_m);
    }
    return res;
}
