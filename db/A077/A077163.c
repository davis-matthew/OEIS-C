/*
 * A077163  n-th power of next n numbers.
 * https://oeis.org/A077163
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^round(sqrt(2*n))
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 23, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note:  round(sqrt(2*n)) is the integer nearest to sqrt(2*n). */

#include <stdint.h>

/* Convert idx_t to seq_t: casting is sufficient because seq_t is
   an unsigned integer type or an arbitrary precision type. */
static seq_t pow_seq_t(seq_t base, uint64_t exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1U) result *= base;
        base *= base;
        exp >>= 1U;
    }
    return result;
}

/* Integer square root by binary search on 0..2^32-1 for target <= 2^64-2.
   Uses __int128 to avoid overflow of mid*mid. */
static uint64_t floor_sqrt_u128(__int128 target)
{
    uint64_t low = 0, high = ((uint64_t)1 << 32) - 1;
    while (low <= high)
    {
        uint64_t mid = (low + high) >> 1;
        __int128 sq = (__int128)mid * mid;
        if (sq <= target)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return low - 1;
}

/* Compute round(sqrt(2*n)) without floating point. */
static uint64_t round_sqrt2(idx_t n)
{
    /* Compute floor sqrt(2*n) */
    __int128 two_n = (__int128)2 * n;          /* may need 128‑bit */
    uint64_t f = floor_sqrt_u128(two_n);      /* f = ⌊√(2n)⌋ */

    /* Rounding rule: if 8*n >= (2*f+1)^2 then round up. */
    __int128 lhs = (__int128)8 * n;            /* 8n */
    __int128 rhs = (__int128)(2 * f + 1) * (2 * f + 1);  /* (2f+1)^2 */

    if (lhs >= rhs) return f + 1;
    else            return f;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    uint64_t exp = round_sqrt2(n);
    seq_t base = (seq_t)n;
    return pow_seq_t(base, exp);
}
