/*
 * A141675  Triangle, T(n, k) = n*(n-k+1)/(k+1) if n mod k+1 = 0, otherwise
 *          T(n, k) = n-k+1, read by rows.
 * https://oeis.org/A141675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = n*(n-k+1)/(k+1) if n mod (k+1) == 0 n-k+1 otherwise The
 *            sequence a(n) is the triangle read by rows, starting with a(1)=1
 * Range:     u8 n <= 276, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* Computes a(n) for n >= 1.
   Uses binary search to find the row r such that
   offset_row(r) ≤ n < offset_row(r+1), where
   offset_row(r) = 1 + r*(r-1)/2.  */
seq_t a(idx_t n)
{
    /* Find row r */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        /* offset_row(mid) = 1 + mid*(mid-1)/2 */
        unsigned __int128 prod = (unsigned __int128)mid * (mid - 1) / 2 + 1;
        if (prod <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t r = low;

    /* Compute column index k (1‑based) */
    unsigned __int128 off = (unsigned __int128)r * (r - 1) / 2;
    idx_t k = n - (idx_t)off;      /* 1 ≤ k ≤ r */

    /* Evaluate T(r,k) */
    if (r % (k + 1) == 0) {
        seq_t numerator = (seq_t)r * ((((seq_t)r) - (seq_t)k) + 1);
        return numerator / (seq_t)(k + 1);
    } else {
        return (seq_t)r - (seq_t)k + 1;
    }
}
