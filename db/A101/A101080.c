/*
 * A101080  Table of Hamming distances between binary vectors representing i
 *          and j, for i >= 0, j >= 0, read by antidiagonals.
 * https://oeis.org/A101080
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = popcount of i XOR j, where (i,j) is the pair reached by
 *            reading the infinite table of Hamming distances in antidiagonal
 *            order
 * Range:     u8 n <= 20300, u16 n <= 20300, u32 n <= 20300, u64 n <= 20300,
 *            u128 n <= 20300, big any n
 * Validated: standard, pass, 121 terms through n = 9730 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Popcount for 64-bit values */
static uint8_t popcount_uint64(uint64_t x) {
    uint8_t c = 0;
    while (x) {
        x &= x - 1;
        c++;
    }
    return c;
}

static inline unsigned long long triangular_threshold(idx_t n)
{
    /* Find the smallest integer t such that t*(t+1)/2 > n.
       Binary search on the 64‑bit domain. */
    unsigned long long low = 0, high = (1ULL << 32); /* upper bound is sufficient */
    while (low < high) {
        unsigned long long mid = (low + high) >> 1;
        unsigned __int128 val = (unsigned __int128)mid * (mid + 1) / 2;
        if (val > n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    unsigned long long t = triangular_threshold(n);
    unsigned long long s = t - 1;                       /* sum i + j on this diagonal */
    unsigned __int128 prefix128 = (unsigned __int128)s * (s + 1) / 2; /* count before this diagonal */
    idx_t r = n - (idx_t)prefix128;                    /* index within the diagonal (i) */

    idx_t i = r;
    idx_t j = s - r;
    idx_t x = i ^ j;                                   /* XOR of the two indices */

    return popcount_uint64((uint64_t)x);
}
