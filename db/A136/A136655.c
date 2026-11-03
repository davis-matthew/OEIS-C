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
 * Validated: standard, pass, 82 terms through n = 9867 (2026-11-01)
 * Helpers:   ipow_i32, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/ipow_i32.h"
#include "../utils/ipow_idx.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

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
        res = ipow_idx(m, tau / 2);
    }
    else /* tau odd: m must be a perfect square */
    {
        uint64_t sqrt_val = (uint64_t)floor_sqrt(m);
        seq_t sqrt_m = (seq_t)sqrt_val;
        res = ipow_idx(m, (tau - 1) / 2);
        res = seq_mul(res, sqrt_m);
    }
    return res;
}
