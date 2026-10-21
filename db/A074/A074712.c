/*
 * A074712  Number of (interiors of) cells touched by a diagonal in a regular
 *          n X k grid (enumerated antidiagonally).
 * https://oeis.org/A074712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl, look
 * Formula:   a(n) = A(i,k) where i = t+1, k = m - t, m = smallest integer
 *            with m*(m+1)/2 >= n, t = n - ((m-1)*m/2) - 1. A(i,k) = i + k -
 *            gcd(i,k)
 * Range:     u8 n <= 9794, u16 n <= 9794, u32 n <= 9794, u64 n <= 9794,
 *            u128 n <= 9794, big any n
 * Helpers:   diagonal_from_idx, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/diagonal_from_idx.h"

seq_t a(idx_t n) {
    if (n < 1) return 0; /* undefined for n<1 */
    idx_t m = diagonal_from_idx(n);
    __uint128_t prev = (__uint128_t)(m - 1) * m / 2;
    idx_t offset = n - (idx_t)prev - 1;    /* 0-based within the diagonal */
    idx_t i = offset + 1;
    idx_t k = m - offset;
    seq_t sum = (seq_t)i + (seq_t)k;
    seq_t g = gcd((seq_t)i, (seq_t)k);
    return sum - g;
}
