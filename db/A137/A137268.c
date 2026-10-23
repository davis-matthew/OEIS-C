/*
 * A137268  Triangle T(n, k) = k! * (k+1)^(n-k), read by rows.
 * https://oeis.org/A137268
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 39, u32 n <= 87, u64 n <= 224,
 *            u128 n <= 616, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle T(n, k) = k! * (k+1)^(n-k) read by rows.
   a(n) is the nth element of the flattened triangle
   (n starts at 1).  For given linear index n, find the
   row r and column k such that n = r(r-1)/2 + k, 1 <= k <= r.
   Then a(n) = k! * (k+1)^(r-k).  See OEIS A137268. */

#include <stdint.h>
#include <limits.h>
#include <stdbool.h>

/* Compute the nth term of the triangle, flattened row-wise. */
seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 >= n and (r-1)r/2 < n. */
    idx_t low = 1;
    idx_t high = n;                /* r cannot exceed n */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 val = (unsigned __int128)mid * (mid + 1) / 2;
        if (val >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* k = offset within the row */
    unsigned __int128 tmp = (unsigned __int128)(r - 1) * r / 2;
    idx_t tri_prev = (idx_t)tmp;
    idx_t k = n - tri_prev;        /* 1 <= k <= r */

    /* Compute k! */
    seq_t fact = 1;
    for (idx_t i = 2; i <= k; ++i) {
        fact *= (seq_t)i;
    }

    /* Compute (k+1)^(r-k) by repeated multiplication (r-k <= r <= n). */
    seq_t pow = 1;
    seq_t base = (seq_t)(k + 1);
    for (idx_t i = 0; i < (r - k); ++i) {
        pow *= base;
    }

    /* Result */
    seq_t result = fact * pow;
    return result;
}
