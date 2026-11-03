/*
 * A187485  Positions of 0's in A123740; complement of A003623.
 * https://oeis.org/A187485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 92, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 108 terms through n = 8996 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = floor(ceil(n/2)*phi) + floor(n/2)
 *
 * This uses the integer identity
 *   floor(k*phi) = (k + floor(k*sqrt(5))) / 2
 * with phi = (1+sqrt(5))/2, avoiding any floating point arithmetic.
 */
#include <stdint.h>
#include "../utils/ipow_i32.h"

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

static seq_t floor_k_phi(idx_t k)
{
    seq_t kseq = (seq_t)k;
    /* compute floor(k*sqrt(5)) as floor_sqrt(k*k*5) */
    seq_t sq = kseq * kseq * (seq_t)5;
    seq_t sqrt5k = floor_sqrt(sq);
    return (kseq + sqrt5k) / (seq_t)2;
}

seq_t a(idx_t n)
{
    if (n == 0) {
        /* undefined for n=0; return 0 as a placeholder */
        return (seq_t)0;
    }
    idx_t half_floor = n / 2;          /* floor(n/2)   */
    idx_t half_ceil  = (n + 1) / 2;    /* ceil(n/2)    */
    seq_t part = floor_k_phi(half_ceil);
    return part + (seq_t)half_floor;
}
