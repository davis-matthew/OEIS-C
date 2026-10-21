/*
 * A081517  Consider the smallest number m which can be expressed as the sum
 *          of n distinct numbers coprime to m. Sequence gives triangle (read
 *          by rows) of the set of coprime numbers pertaining to m. When there
 *          is a choice, use the lexicographically earliest solution.
 * https://oeis.org/A081517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(1) = 1 For n ≥ 2 let r be the smallest integer with r·(r+1)/2
 *            ≥ n. If the position in that row is ≤ r−1, then a(n) = that
 *            position. Otherwise a(n) = p − (r−1)·r/2 where p is the least
 *            prime ≥ r·(r+1)/2. This implements the OEIS triangle read by
 *            rows
 * Range:     u8 n <= 1769, u16 n <= 7260, u32 n <= 7260, u64 n <= 7260,
 *            u128 n <= 7260, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)1;

    /* Find row r: smallest r such that r*(r+1)/2 ≥ n. */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __uint128_t t = (__uint128_t)mid * (mid + 1) / 2;
        if (t >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* Sum of elements in all previous rows: (r-1)*r/2. */
    idx_t prev_sum = (idx_t)((__uint128_t)(r - 1) * r / 2);
    idx_t col = n - prev_sum;  /* 1‑based column index within row r. */

    if (col <= r - 1)
        return (seq_t)col;  /* lexicographically earliest choice. */

    /* Compute the least prime ≥ r*(r+1)/2. */
    seq_t target = (seq_t)r * ((seq_t)r + 1) / 2;
    seq_t m = target;
    while (!is_prime_big(m))
        ++m;

    return (m - (seq_t)prev_sum);
}
