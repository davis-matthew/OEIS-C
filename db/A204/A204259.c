/*
 * A204259  Matrix given by f(i,j) = 1 + [(2i+j) mod 3], by antidiagonals.
 * https://oeis.org/A204259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 1 + ((2*i + j) mod 3), where (i,j) are the * antidiagonal
 *            coordinates of the n‑th element of the infinite * matrix defined
 *            by f(i,j) = 1 + ((2i + j) mod 3). * Off‑set is 1
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer sqrt: floor( sqrt(x) ), for 0 <= x < 2^64 */
static uint64_t floor_sqrt_uint64(uint64_t x)
{
    if (x < 2) return x;
    uint64_t low = 1, high = x >> 1, mid, res = 0;
    while (low <= high) {
        mid = low + ((high - low) >> 1);
        __int128 sq = (__int128)mid * mid;
        if (sq == x) return mid;
        if (sq < x) {
            res = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* under‑offset values are irrelevant */

    /* Find diagonal d: minimal d with d*(d+1)/2 >= n */
    uint64_t d = (floor_sqrt_uint64(8 * n + 1) - 1) >> 1;
    while (((__int128)d * (d + 1)) / 2 < n) d++;

    /* Elements before diagonal d */
    uint64_t prev = ((__int128)(d - 1) * d) / 2;

    /* Position within the diagonal (1‑based) */
    uint64_t t = n - prev;

    /* Antidiagonal coordinates */
    uint64_t i = t;
    uint64_t j = d - t + 1;

    /* Compute the value */
    uint64_t val = 1 + ((2 * i + j) % 3);
    return (seq_t)val;
}
