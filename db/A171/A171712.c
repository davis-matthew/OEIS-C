/*
 * A171712  Triangle T(n,k) read by rows. Coloring of sectors in a circle.
 * https://oeis.org/A171712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the nth entry of the triangle T(n,k) where T(n,1)=1,
 *            T(n,n)=(5-(-1)^n)/2 (for n≥2), and for 1<k<n T(n,k)=(3+(-1)^k)/2
 * Range:     u8 n <= 5050, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 ≥ n and (r-1)r/2 < n. */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* Compute start index of row r: (r-1)*r/2 + 1. */
    unsigned __int128 start128 = (unsigned __int128)(r - 1) * r / 2 + 1;
    idx_t offset = (idx_t)start128;          // fits in idx_t because offset ≤ n
    idx_t k = n - offset + 1;                // position in the row (1‑based)

    /* Determine the value at position (r,k). */
    if (r == 1)
        return (seq_t)1;                     // the single element of row 1

    if (k == r) {                           // last element of the row
        return (seq_t)((r & 1) ? 3 : 2);    // 3 if r odd, else 2
    }

    /* Interior elements: odd => 1, even => 2. */
    return (seq_t)((k & 1) ? 1 : 2);
}
