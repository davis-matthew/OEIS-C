/*
 * A176809  Triangle read by rows: T(n,m) = 1 - (ceiling((n-1)/m) +
 *          ceiling((n-1)/(n-m+1))) + ceiling((n-1)/1) +
 *          ceiling((n-1)/(n-1+1)).
 * https://oeis.org/A176809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,c) where r = smallest integer s.t. r(r+1)/2 ≥ n, c =
 *            n - r(r-1)/2, and T(r,c) = r + ceil((r-1)/r) - ceil((r-1)/c) -
 *            ceil((r-1)/(r-c+1))
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t find_row(idx_t n)
{
    idx_t low = 1, high = n;  /* high is safe for n≥1 */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __int128 tri = (__int128)mid * (mid + 1) / 2;
        if (tri >= n) high = mid;
        else low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Map linear index n to (r,c). */
    idx_t r = find_row(n);
    idx_t prev_tri = r * (r - 1) / 2;
    idx_t c = n - prev_tri;                 /* 1‑based column */

    /* Compute ceil divisions. */
    idx_t ceil1 = (r - 1 + c - 1) / c;                  /* ceil((r-1)/c)   */
    idx_t denom2 = r - c + 1;                           /* (r-c+1) > 0     */
    idx_t ceil2 = (r - 1 + denom2 - 1) / denom2;        /* ceil((r-1)/(r-c+1)) */

    /* Compute T(r,c) = r + ceil((r-1)/r) - ceil1 - ceil2. */
    __int128 term = (__int128)r;
    if (r > 1)        /* ceil((r-1)/r) is 1 for r>1, 0 for r=1 */
        term += 1;
    term -= ceil1;
    term -= ceil2;

    return (seq_t)term;
}
