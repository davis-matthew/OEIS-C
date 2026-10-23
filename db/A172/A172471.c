/*
 * A172471  a(n) = floor(sqrt(2*n)).
 * https://oeis.org/A172471
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(sqrt(2*n))
 * Range:     u8 n <= 9427, u16 n <= 9427, u32 n <= 9427, u64 n <= 9427,
 *            u128 n <= 9427, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    // Compute 2*n using 128‑bit arithmetic to avoid overflow
    unsigned __int128 prod = (unsigned __int128)n * 2u;

    // Binary search for the largest x such that x*x <= 2*n
    idx_t lo = 0;
    idx_t hi = (idx_t)1 << 33;   // Upper bound > sqrt(2*n) for all n < 2^64

    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;

        if (sq <= prod) {
            lo = mid + 1;        // mid is a candidate; try larger
        } else {
            if (mid == 0) break; // prevent underflow
            hi = mid - 1;        // mid is too large; try smaller
        }
    }

    // lo is the smallest number with lo*lo > 2*n, so result is lo-1
    return (seq_t)(lo - 1);
}
