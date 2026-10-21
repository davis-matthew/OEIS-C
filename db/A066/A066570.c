/*
 * A066570  Product of numbers <= n that have a prime factor in common with n.
 * https://oeis.org/A066570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = product_{k ≤ n, gcd(k,n) > 1} k (empty product = 1)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 17, u64 n <= 29, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Euclidean algorithm for idx_t */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;   /* empty product */

    seq_t res = (seq_t)1;
    for (idx_t k = 2; k <= n; ++k) {
        if (gcd_idx(k, n) > 1)
            res *= (seq_t)k;
    }
    return res;
}
