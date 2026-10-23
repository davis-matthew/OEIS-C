/*
 * A182008  a(n) = ceiling(sqrt(2*n*log(2))).
 * https://oeis.org/A182008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(sqrt(2*n*log(2)))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* logarithm of 2 approximated to 18 decimal digits.  */
static const unsigned long long LOG2_NUM = 693147180559945309ULL;
static const unsigned long long LOG2_DEN = 1000000000000000000ULL; /* 1e18 */

/* 2^63 used as a scaling factor.  */
static const unsigned __int128 LOG2_SCALE = ((unsigned __int128)1) << 63;

/* floor(log(2) * 2^63) computed once.  */
static const unsigned __int128 LOG2_L = 
    ((unsigned __int128)LOG2_NUM * LOG2_SCALE) / LOG2_DEN;

seq_t a(idx_t n)
{
    /* sequence is defined for n >= 1; return 0 for n == 0. */
    if (n == 0) return 0;

    /* Compute N = 2 * n * floor(log(2) * 2^63) as a 128‑bit integer. */
    unsigned __int128 N = (unsigned __int128)n * 2 * LOG2_L;

    /* target = ceil(N / 2^63).  */
    unsigned __int128 q = N / LOG2_SCALE;
    unsigned __int128 r = N % LOG2_SCALE;
    unsigned __int128 target = q + (r ? 1 : 0);  /* ceil division */

    /* Binary search for the least k such that k^2 >= target. */
    idx_t low  = 1;
    idx_t high = (n > 2) ? n : 2;  /* ensure the interval contains the answer */

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 mid2 = (unsigned __int128)mid * mid;
        if (mid2 >= target)
            high = mid;
        else
            low = mid + 1;
    }

    return (seq_t)low;
}
