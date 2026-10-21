/*
 * A059543  Beatty sequence for log(3).
 * https://oeis.org/A059543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n * log(3)) (log(3) ≈ 1.0986122886681098...)
 * Range:     u8 n <= 233, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Precision: 48 fractional bits (2^48 ≈ 2.8×10^14) */
static const unsigned int   LOG3_DEN_SHIFT = 48;
static const unsigned long long LOG3_NUM =
    (unsigned long long)((long double)1.0986122886681098L *
                         (long double)((unsigned long long)1ULL << LOG3_DEN_SHIFT) + 0.5L);

seq_t a(idx_t n)
{
    /* Compute n * LOG3_NUM using unsigned 128‑bit arithmetic to prevent overflow
       (n ≤ 2^64–1 and LOG3_NUM < 2^49, so the product fits in 113 bits) */
    unsigned __int128 prod = (unsigned __int128)n * LOG3_NUM;
    /* Floor division by 2^48 is a right shift */
    return (seq_t)(prod >> LOG3_DEN_SHIFT);
}
