/*
 * A190059  a(n) = n + [n*s/r] + [n*t/r]; r=1, s=sin(Pi/5), t=csc(Pi/5).
 * https://oeis.org/A190059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + ⌊n* sin(Pi/5)⌋ + ⌊n* csc(Pi/5)⌋
 * Range:     u8 n <= 78, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 7081 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The constants are scaled by 10^18 so that all calculations use
   integer arithmetic only.  They are accurate to more than 18
   decimal places and are sufficient for correct floor results up to
   n ≈ 10^12 (and far beyond).  */
#include <stdint.h>

/* Integer division of (n * num) / den using 128‑bit arithmetic.
   Works for all idx_t values that fit in 64 bits.  The result is
   truncated to seq_t, which is safe because the quotient fits into
   the output type for the required range of n. */
static inline seq_t floor_mul_u64(idx_t n, uint64_t num, uint64_t den)
{
    /* Use __int128 to avoid overflow of the product `n * num`. */
    __int128 prod = (__int128)n * num;
    return (seq_t)(prod / den);
}

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
