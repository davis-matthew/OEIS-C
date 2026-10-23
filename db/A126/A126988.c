/*
 * A126988  Triangle read by rows: T(n,k) = n/k if k is a divisor of n; T(n,k)
 *          = 0 if k is not a divisor of n (1 <= k <= n).
 * https://oeis.org/A126988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = n-th entry of the triangle T where * T(r,c) = r/c if c
 *            divides r, otherwise 0. * The triangle is read row by row
 *            starting at n = 1
 * Range:     u8 n <= 7875, u16 n <= 7875, u32 n <= 7875, u64 n <= 7875,
 *            u128 n <= 7875, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#ifndef NO_IMPLEMENTATION
seq_t a(idx_t n)
{
    /* Find the row r such that (r-1)*r/2 < n <= r*(r+1)/2 .
       Use binary search on 64‑bit indices, computing the triangular
       numbers in __int128 to avoid overflow. */
    idx_t low = 1, high = n;          /* high can be n, safe for binary search */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __int128 tri = (__int128)mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* Position within the row: column index c. */
    __int128 prev = (__int128)(r - 1) * r / 2;
    idx_t c = n - (idx_t)prev;        /* 1‑based column index */

    /* Compute the triangle entry: r/c if c divides r, else 0. */
    seq_t result = 0;
    if (c != 0 && r % c == 0)
        result = (seq_t)(r / c);
    return result;
}
#else
NO_IMPLEMENTATION: The triangle T has no known closed form, so the sequence is computed by explicit indexing.
#endif
