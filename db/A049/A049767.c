/*
 * A049767  Triangular array T, read by rows: T(n,k) = (k^2 mod n) + (n^2 mod
 *          k), for k = 1..n and n >= 1.
 * https://oeis.org/A049767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the value obtained by reading the triangular array
 *            T(n,k) = (k^2 mod n) + (n^2 mod k) row by row. For a given n,
 *            find the unique pair (r,k) such that n = r(r-1)/2 + k, 1 ≤ k ≤ r
 *            and return T(r,k)
 * Range:     u8 n <= 5050, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t find_row(idx_t n)
{
    idx_t r = 0;
    idx_t sum = 0;
    /* Since sum grows as r^2/2, while n ≤ 2^64-1,
       a linear scan up to ~10^6 steps is fine. */
    while (sum < n) {
        ++r;
        sum += r;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined, but harmless */

    idx_t r = find_row(n);            /* row number (1-indexed) */
    idx_t k = n - ((r - 1) * r) / 2;  /* column number (1-indexed) */

    /* Compute k^2 mod r and r^2 mod k. Use 128-bit intermediates
       to avoid overflow on 64-bit idx_t. */
    __uint128_t k2 = (__uint128_t)k * k;
    seq_t part1 = (seq_t)(k2 % r);

    __uint128_t r2 = (__uint128_t)r * r;
    seq_t part2 = (seq_t)(r2 % k);

    return part1 + part2;
}
