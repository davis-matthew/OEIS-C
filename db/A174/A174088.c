/*
 * A174088  Number of pairs (i,j) such that i*j == 0 (mod k), 0 <= i <= j < k.
 * https://oeis.org/A174088
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of pairs (i,j), 0 ≤ i ≤ j < n, with i*j ≡ 0 (mod
 *            n) Computed in O(n) time using gcd and simple arithmetic
 * Range:     u8 n <= 83, u16 n <= 6299, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* Ensure n >= 1 in normal usage; if n == 0 return 0 */
    if (n == 0) return (seq_t)0;
    seq_t res = (seq_t)n;          /* pairs with i = 0 */
    for (idx_t i = 1; i < n; ++i) {
        idx_t g = gcd_idx(i, n);   /* gcd(i,n) */
        idx_t m = n / g;           /* smallest multiple of m divides n */
        /* smallest j ≥ i that is multiple of m: ceil division */
        idx_t t0 = (i + m - 1) / m;
        if (t0 < g)                /* there exist j values ≥ i */
            res += (seq_t)(g - t0);
    }
    return res;
}
