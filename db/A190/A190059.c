/*
 * A190059  a(n) = n + [n*s/r] + [n*t/r]; r=1, s=sin(Pi/5), t=csc(Pi/5).
 * https://oeis.org/A190059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + ⌊n* sin(Pi/5)⌋ + ⌊n* csc(Pi/5)⌋
 * Range:     u8 n <= 78, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_mul_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The constants are scaled by 10^18 so that all calculations use
   integer arithmetic only.  They are accurate to more than 18
   decimal places and are sufficient for correct floor results up to
   n ≈ 10^12 (and far beyond).  */
#include <stdint.h>
#include "../utils/floor_mul_u64.h"

static const uint64_t SIN_NUM   = 587785252292473129ULL; /* sin(Pi/5) * 1e18  */
static const uint64_t SIN_DEN   = 1000000000000000000ULL;/* 1e18              */
static const uint64_t CSC_NUM   = 1701301945133573400ULL;/* csc(Pi/5) * 1e18  */
static const uint64_t CSC_DEN   = 1000000000000000000ULL;/* 1e18              */

seq_t a(idx_t n)
{
    seq_t res = (seq_t)n;
    res += floor_mul_u64(n, SIN_NUM, SIN_DEN);
    res += floor_mul_u64(n, CSC_NUM, CSC_DEN);
    return res;
}
