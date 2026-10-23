/*
 * A111805  Number triangle T(n,k)=binomial(2(n+k),4k).
 * https://oeis.org/A111805
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(r,k) where r = ⌊(√(8n+1)−1)/2⌋, k = n−r(r+1)/2 and
 *            T(r,k) = binomial(2(r+k), 4k). The sequence is the rows of the
 *            triangle flattened left‑to‑right
 * Range:     u8 n <= 11, u16 n <= 30, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   binomial_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial_seq.h"

/* Compute the greatest r such that r(r+1)/2 ≤ n. */
static idx_t row_of_index(idx_t n)
{
    idx_t low = 0, high = 1;

    /* Find an upper bound for r. */
    while ((unsigned __int128)high * (high + 1) / 2 <= n) {
        /* Prevent infinite loop if high overflows; stop if doubling
           would exceed n. */
        if (high > (UINT64_MAX >> 1)) break;
        high <<= 1;
    }

    /* Binary search between low and high. */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        unsigned __int128 val = (unsigned __int128)mid * (mid + 1) / 2;
        if (val <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Determine the row r and column k for index n. */
    idx_t r = row_of_index(n);
    unsigned __int128 base128 = (unsigned __int128)r * (r + 1) / 2;
    idx_t base = (idx_t)base128;
    idx_t k = n - base;

    /* Compute the binomial coefficient T(r,k)=C(2(r+k),4k). */
    idx_t N = 2 * (r + k);
    idx_t K = 4 * k;
    return binomial_seq(N, K);
}
