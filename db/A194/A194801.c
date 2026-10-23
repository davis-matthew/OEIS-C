/*
 * A194801  Square array read by antidiagonals: T(n,k) = k*((n+1)*k-n+1)/2, k
 *          = 0, +- 1, +- 2,..., n >= 0.
 * https://oeis.org/A194801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A194801: Square array read by antidiagonals  
   T(n,k) = k * ((n+1)*k - n + 1) / 2   for n ≥ 0, k = 0, ±1, ±2, …  
   The sequence is obtained by listing the values T(n,k) in antidiagonal
   order: for each index m (starting at 0) compute the row n and a
   column index j; from j recover the signed k.   */

#include <stdint.h>
#include <math.h>

/* Convert a sequence index n (0‑based) to the array value.   */
seq_t a(idx_t n)
{
    /* 1. Find the antidiagonal index d such that
       t(d) = d*(d+1)/2 ≤ n < t(d+1).   */
    idx_t d = (idx_t)((sqrtl((long double)(8.0L*n + 1.0L)) - 1.0L) / 2.0L);
    idx_t t = d * (d + 1) / 2;
    /* Correct possible rounding errors.   */
    while (t > n) {
        d--;
        t = d * (d + 1) / 2;
    }
    while ((d + 1) * (d + 2) / 2 <= n) {
        d++;
        t = d * (d + 1) / 2;
    }

    /* 2. Compute offset within this diagonal.   */
    idx_t offset = n - t;

    /* 3. Convert antidiagonal coordinates to (row, column).   */
    idx_t row = d - offset;   /* n in T(n,k)   */
    idx_t col = offset;       /* column index   */

    /* 4. Recover the signed column index k:
           col 0 → k=0
           col odd  → k = (col+1)/2
           col even → k = -col/2   (col=0 handled above)   */
    int64_t k;
    if (col & 1ULL) {
        k = (int64_t)((col + 1ULL) / 2ULL);
    } else {
        k = - (int64_t)(col / 2ULL);
    }

    /* 5. Evaluate T(row,k) using 128‑bit arithmetic to avoid overflow
       on intermediate products.   */
    int64_t rr = (int64_t)row;
    __int128 temp = (__int128)(rr + 1) * k - rr + 1;
    __int128 val128 = (__int128)k * temp / 2;
    return (seq_t)val128;   /* result is non‑negative */
}
