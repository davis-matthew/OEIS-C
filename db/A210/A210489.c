/*
 * A210489  Array read by ascending antidiagonals where row n contains the
 *          second partial sums of row n of Pascal's triangle.
 * https://oeis.org/A210489
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(i,j) T(i,j) = Σ_{k=1..j} k * binomial(i, j-k) (i,j) are
 *            obtained by reading the table of T in ascending antidiagonals:
 *            let t = ⌊(√(8n+1)-1)/2⌋, off = n - t(t+1)/2, then i = t - off, j
 *            = off + 1
 * Range:     u8 n <= 81, u16 n <= 305, u32 n <= 1140, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

static seq_t T(idx_t n, idx_t m)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= m; ++k) {
        seq_t bin = binomial(n, m - k);
        sum += bin * (seq_t)k;
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* find the largest t such that t(t+1)/2 <= n */
    idx_t low = 0, high = (idx_t)1 << 33; /* > sqrt(2*max_uint64) */
    idx_t t = 0;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        __uint128_t val = (__uint128_t)mid * (mid + 1) / 2;
        if (val <= n) {
            t = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break; /* avoid underflow */
            high = mid - 1;
        }
    }

    idx_t L = t + 1;
    idx_t cumPrev = (idx_t)((__uint128_t)t * (t + 1) / 2);
    idx_t off = n - cumPrev;

    idx_t i = t - off;
    idx_t m = off + 1;
    return T(i, m);
}
