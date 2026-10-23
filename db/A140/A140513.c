/*
 * A140513  Repeat 2^n n times.
 * https://oeis.org/A140513
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 2^k where k = floor((sqrt(8*n+1)+1)/2)
 * Range:     u8 n <= 27, u16 n <= 119, u32 n <= 495, u64 n <= 2015,
 *            u128 n <= 8127, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square‑root of a 128‑bit unsigned value */
static inline uint64_t isqrt128(unsigned __int128 v)
{
    uint64_t low  = 0;
    uint64_t high = ((uint64_t)1 << 35);   /* > sqrt(8*2^64) */
    uint64_t best = 0;
    while (low <= high) {
        uint64_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= v) {
            best = mid;
            low  = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return best;
}

/* compute 2^exp in type seq_t (works for all unsigned integer
   types and boost::multiprecision::cpp_int) */
static inline seq_t pow2_seq(idx_t exp)
{
    return ((seq_t)1) << exp;
}

seq_t a(idx_t n)
{
    /* compute d = floor(sqrt(8*n + 1)) */
    unsigned __int128 v = (unsigned __int128)8 * n + 1;
    uint64_t d = isqrt128(v);

    /* k = floor((d + 1) / 2) */
    uint64_t k = (d + 1) / 2;

    return pow2_seq((idx_t)k);
}
