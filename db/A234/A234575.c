/*
 * A234575  Triangle T(n, k) read by rows: T(n, k) = floor(n/k) + n mod k.
 * https://oeis.org/A234575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = floor(r/k) + r % k where r, k are the row and column of n
 *            in the triangle T(n, k) read by rows. Here r is the smallest
 *            integer such that r*(r+1)/2 ≥ n, and k = n - (r-1)*r/2
 * Range:     u8 n <= 8709, u16 n <= 8709, u32 n <= 8709, u64 n <= 8709,
 *            u128 n <= 8709, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Find the row index r for the given overall index n (n ≥ 1). */
static idx_t find_row(idx_t n)
{
    idx_t low = 1, high = 1;
    /* Expand high until the triangular number is ≥ n. */
    while ((__int128)high * (high + 1) / 2 < n)
        high <<= 1;
    /* Binary search for minimal r with r*(r+1)/2 ≥ n. */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((__int128)mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* Main product function. */
seq_t a(idx_t n)
{
    /* Since the OEIS offset is 1, n is at least 1. */
    idx_t r = find_row(n);
    /* Elements before row r: (r-1)*r/2. */
    idx_t prev_total = (idx_t)((__int128)(r - 1) * r / 2);
    idx_t k = n - prev_total;          /* 1 ≤ k ≤ r */
    idx_t div = r / k;
    idx_t mod = r % k;
    return (seq_t)div + (seq_t)mod;
}
