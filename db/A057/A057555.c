/*
 * A057555  Lexicographic ordering of N x N, where N = {1,2,3...}.
 * https://oeis.org/A057555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) returns the nth element of the flattened lexicographic
 *            ordering of lattice points (i,j) with i,j >= 1. For odd n the
 *            function returns the first coordinate i, for even n it returns
 *            the second coordinate j of the pair with index k = ceil(n/2).
 *            The pair (i,j) satisfies the inverse of the positi...
 * Range:     u8 n <= 156, u16 n <= 156, u32 n <= 156, u64 n <= 156,
 *            u128 n <= 156, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t isqrt_idx(idx_t x)
{
    idx_t low = 0, high = x;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return high;
}

seq_t a(idx_t n)
{
    // pair index k = ceil(n/2)  (integer arithmetic)
    idx_t k = (n + 1) / 2;

    // compute q = floor((1 + sqrt(8k-7)) / 2)
    idx_t t = 8 * k - 7;                 /* 8k-7 is non‑negative for k>=1   */
    idx_t sqrt_t = isqrt_idx(t);
    idx_t q = (sqrt_t + 1) / 2;

    /* compute i = k - q*(q-1)/2, j = q + 1 - i */
    seq_t half = (seq_t)q * (q - 1) / 2;
    seq_t i = (seq_t)k - half;
    seq_t j = (seq_t)q + 1 - i;

    /* return first coordinate on odd n, second on even n */
    return (n & 1) ? i : j;
}
