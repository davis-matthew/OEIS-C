/*
 * A135296  A measure of quality (the higher the better) for the approximation
 *          to Pi by rationals A022853(n)/n.
 * https://oeis.org/A135296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = floor(1 / | n*π - round(n*π) |)
 * Range:     u8 n <= 112, u16 n <= 2500, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* 10^15 is the denominator used to represent π as an integer fraction.
   PI_NUM = floor(π * 10^15) = 3141592653589793.   */
static const uint64_t PI_NUM = 3141592653589793ULL;
static const uint64_t PI_DEN = 1000000000000000ULL;

seq_t a(idx_t n)
{
    /* Compute n * PI_NUM in 128‑bit to avoid overflow */
    __int128 np = (__int128)n * PI_NUM;

    /* k = nearest integer to nπ (all integer arithmetic) */
    uint64_t k = (uint64_t)((np + PI_DEN / 2) / PI_DEN);

    /* r = | n*PI_NUM - k*PI_DEN |   (distance times denominator) */
    __int128 diff = np - (__int128)k * PI_DEN;
    uint64_t r = diff >= 0 ? (uint64_t)diff : (uint64_t)(-diff);

    /* a(n) = floor(PI_DEN / r)   because 1/|nπ-k| = PI_DEN/r */
    if (r == 0) return (seq_t)0;   /* Theoretical impossibility, safety */
    return (seq_t)(PI_DEN / r);
}
