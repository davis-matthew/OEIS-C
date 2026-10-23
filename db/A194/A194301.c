/*
 * A194301  Triangular array: g(n,k)=number of fractional parts (i*sqrt(5)) in
 *          interval [(k-1)/n, k/n], for 1<=i<=n, 1<=k<=n.
 * https://oeis.org/A194301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A194301:
   g(n,k) = number of i (1 ≤ i ≤ n) such that
   fractional part of i·√5 lies in [(k-1)/n , k/n].
   The sequence a(n) lists the array g row‑wise, starting with a(1)=g(1,1). */

#include <math.h>   /* for sqrtl, floorl */

/* integer square root: floor(√x) for x < 2^64 */
static inline idx_t floor_sqrt_u64(idx_t x) {
    idx_t low = 0, high = (x >> 1) + 1, mid;
    while (low < high) {
        mid = (low + high + 1) >> 1;
        if (mid > 0 && mid > x / mid)
            high = mid - 1;
        else
            low = mid;
    }
    return low;
}

/* Main routine for A194301 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety for out‑of‑range calls */

    /* -----------------------------------------------------------------
       Locate the row r and column k that the n‑th element falls into.
       The triangular table has rows of lengths 1,2,3,…; the index n is
       1‑based.  We find the smallest r with r*(r+1)/2 >= n.  */
    idx_t r = (floor_sqrt_u64(8*n + 1) - 1) >> 1;
    if (r * (r + 1) / 2 < n) ++r;

    /* offset of the first element in this row */
    idx_t offset = r * (r - 1) / 2 + 1;
    idx_t k = n - offset + 1;          /* 1‑based column index */

    /* -----------------------------------------------------------------
       Count the number of i in [1..r] with fractional part of i*√5
       falling into the interval [(k-1)/r , k/r].
       Since r ≤ 141 for n ≤ 10 000, we can simply iterate over i.  */
    const long double SQRT5 = sqrtl(5.0L);
    seq_t count = 0;
    for (idx_t i = 1; i <= r; ++i) {
        long double val = i * SQRT5;
        long double frac = val - floorl(val);
        idx_t bucket = (idx_t)(frac * r);  /* 0 … r-1 */
        if (bucket == k - 1) ++count;
    }
    return count;
}
