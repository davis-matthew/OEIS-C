/*
 * A224777  Triangle with integer geometric mean sqrt(n*m) for 1 <= m <= n,
 *          and 0 if sqrt(n*m) is not integer.
 * https://oeis.org/A224777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = sqrt(r*j) if r*j is a perfect square, otherwise 0. For a
 *            given n, find the row r in the triangular array A(i,j)=sqrt(i*j)
 *            (with 1 <= j <= i) such that the index n lies in row r. Row r
 *            contains exactly r entries, so the first index of row r is
 *            T_{r-1}+1 where T_k = k*(k+1)/2. Thus r = min {...
 * Range:     u8 n <= 35, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

/* Return the smallest r such that r*(r+1)/2 >= n. */
static inline idx_t triangular_index(idx_t n)
{
    idx_t low = 0, high = 1;
    /* increase high until the triangular number exceeds n */
    while ((__int128)high * (high + 1) / 2 < n) {
        high <<= 1;
    }
    /* binary search in [low,high] */
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        if ((__int128)mid * (mid + 1) / 2 >= n) {
            high = mid;
        } else {
            low = mid;
        }
    }
    return high;
}

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }
    idx_t r = triangular_index(n);
    idx_t tprev = (__int128)(r - 1) * r / 2;
    idx_t j = n - (idx_t)tprev;
    seq_t prod = (seq_t)r * (seq_t)j;
    seq_t s = floor_sqrt(prod);
    if (s * s == prod) {
        return s;
    }
    return 0;
}
