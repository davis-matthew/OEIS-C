/*
 * A140765  Array T(n,k) = binomial(k+2, k-1) + n*binomial(k+2, k) read by
 *          antidiagonals.
 * https://oeis.org/A140765
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   prefix_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A140765: Read array T(n,k) = binomial(k+2,3) + n*binomial(k+2,2) by antidiagonals.
 * a(n) is the n-th element (0‑based) when the array is traversed along antidiagonals
 * with decreasing n (i.e., for each fixed s = n+k, output T(n,k) for n = s, s-1, …, 0).
 *
 * The mapping from index n to (n_row,k) is:
 *   s       = largest integer such that s*(s+1)/2 ≤ n        (antidiagonal sum)
 *   offset  = n - s*(s+1)/2                                 (position within that diagonal, starting at 0)
 *   n_row   = s - offset
 *   k       = offset
 * Then
 *   T = binom3 + n_row * binom2
 *   binom2 = (k+2)*(k+1)/2
 *   binom3 = (k+2)*(k+1)*k/6
 *
 * All arithmetic is performed in a 128‑bit intermediate type to avoid overflow
 * before the result is cast to seq_t.  This keeps the value exact whenever it
 * fits in seq_t and works correctly for arbitrary‑precision seq_t.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/prefix_count.h"

seq_t a(idx_t n)
{
    /* Find the antidiagonal sum s by binary search. */
    idx_t low = 0, high = 1;
    /* Grow high until it exceeds the needed range. */
    while (( (__int128)high * (high + 1) / 2 ) <= n) {
        high <<= 1;               /* double high */
    }
    /* Binary search for largest s with prefix_count(s) <= n. */
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;   /* ceil(mid) */
        if (( (__int128)mid * (mid + 1) / 2 ) <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t s = low;
    idx_t cum_before = prefix_count(s);
    idx_t offset = n - cum_before;
    idx_t n_row = s - offset;
    idx_t k = offset;

    /* Compute binomial coefficients in 128‑bit intermediate. */
    __int128 k128 = (__int128)k;
    __int128 binom2 = ((k128 + 2) * (k128 + 1)) / 2;
    __int128 binom3 = ((k128 + 2) * (k128 + 1) * k128) / 6;

    __int128 res128 = binom3 + (__int128)n_row * binom2;
    return (seq_t)res128;
}
