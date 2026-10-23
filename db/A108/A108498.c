/*
 * A108498  Triangle read by rows: T(n,k) = sum_i{1<=i<=n} k^i mod n, showing
 *          1<=k<=n.
 * https://oeis.org/A108498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r, k) where * r = smallest integer with r(r+1)/2 >= n
 *            (row index) * k = n - r(r-1)/2 (column index) * T(r, k) =
 *            sum_{i=1}^{r} k^i mod r * This computes the flattened A108498
 *            triangle read by rows
 * Range:     u8 n <= 103, u16 n <= 103, u32 n <= 103, u64 n <= 103,
 *            u128 n <= 103, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the row r containing the n-th term (1‑based). */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n) {
        ++r;
    }

    /* Column index k (1‑based). */
    idx_t k = n - (r - 1) * r / 2;

    if (r == 1) {
        /* For modulus 1 every sum is 0. */
        return 0;
    }

    /* Use 64‑bit integers for safe intermediate calculations. */
    uint64_t mod  = (uint64_t)r;
    uint64_t base = (uint64_t)k % mod;
    uint64_t sum  = 0;

    for (idx_t i = 1; i <= r; ++i) {
        sum += base;
        sum %= mod;          /* keep sum within [0, mod-1] */
        base = (base * (uint64_t)k) % mod;
    }

    return (seq_t)sum;
}
