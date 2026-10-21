/*
 * A213852  Least m>0 such that n+1+m and n-m are relatively prime.
 * https://oeis.org/A213852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = least m>0 such that gcd(n+1+m, n-m) = 1
 * Range:     u8 n <= 280, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Find the smallest positive m for which the two integers are coprime. */
    idx_t m = 1;
    for (;; ++m)
    {
        /* Compute n+1+m in the larger integer type. */
        seq_t a_val = (seq_t)n + (seq_t)1 + (seq_t)m;

        /* Compute |n-m|.  Use unsigned arithmetic to avoid negative values. */
        idx_t b_abs = (n >= m) ? (n - m) : (m - n);
        seq_t b_val = (seq_t)b_abs;

        /* If these two numbers are coprime, return the corresponding m. */
        if (gcd(a_val, b_val) == (seq_t)1)
            return (seq_t)m;
    }
}
