/*
 * A226314  Triangle read by rows: T(i,j) = j+(i-j)/gcd(i,j) (1<=i<=j).
 * https://oeis.org/A226314
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, tabl
 * Formula:   a(n) = i - (i-j)/gcd(i,j) where the pair (i,j) is obtained by
 *            reading the triangle T(i,j)=i-(i-j)/gcd(i,j) row‑by‑row: A226314
 *            gives the entry in row i and column j. Here i is the unique row
 *            such that i*(i+1)/2 >= n, and j = n - i*(i-1)/2
 * Range:     u8 n <= 7260, u16 n <= 7260, u32 n <= 7260, u64 n <= 7260,
 *            u128 n <= 7260, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    /* Find row i: smallest i with i*(i+1)/2 >= n. */
    idx_t lo = 1, hi = n;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        idx_t tri = mid * (mid + 1) / 2;
        if (tri >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t i = lo;
    idx_t before = i * (i - 1) / 2;
    idx_t j = n - before; /* j is 1‑based within the row */

    /* Compute the triangle entry T(i,j) = i - (i-j)/gcd(i,j). */
    seq_t g = gcd((seq_t)i, (seq_t)j);       /* gcd of i and j */
    seq_t diff = (seq_t)(i - j);             /* i >= j, so diff >= 0 */
    seq_t sub  = diff / g;                   /* integer division */
    seq_t res  = (seq_t)i - sub;

    return res;
}
