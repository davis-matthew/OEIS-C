/*
 * A183298  Complement of A147875.
 * https://oeis.org/A183298
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(sqrt(2n/5)) if 2n >
 *            floor(sqrt(2n/5))*(5*floor(sqrt(2n/5))+1) otherwise a(n) = n +
 *            floor(sqrt(2n/5)) - 1 (OEIS A183298)
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root for a 64‑bit unsigned integer.
   Returns ⌊√x⌋.  Uses only integer arithmetic. */
static inline idx_t floor_isqrt_u64(idx_t x)
{
    if (x == 0) return 0;
    idx_t low = 1, high = x, mid, sq;
    while (low <= high) {
        mid = low + ((high - low) >> 1);
        sq = mid * mid;
        if (sq == x) return mid;
        if (sq < x) low = mid + 1;
        else high = mid - 1;
    }
    return high;
}

seq_t a(idx_t n)
{
    /* Sequence is defined for n ≥ 1.  For n == 0 the behaviour is
       undefined but we avoid a crash. */
    if (n == 0)
        return (seq_t)0;

    /* Compute intermediate unsigned 64‑bit values. */
    idx_t two_n    = (idx_t)2 * n;          /* 2n   */
    idx_t floor_d  = two_n / (idx_t)5;      /* floor(2n/5) */
    idx_t k        = floor_isqrt_u64(floor_d);  /* floor(sqrt(floor_d)) */
    idx_t right    = k * ( (idx_t)5 * k + (idx_t)1 ); /* k*(5k+1) */

    seq_t base = (seq_t)n + (seq_t)k;       /* n + k */
    /* Return base or base‑1 depending on the comparison. */
    return (seq_t)(two_n > right ? base : base - (seq_t)1);
}
