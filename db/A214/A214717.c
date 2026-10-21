/*
 * A214717  Least m>0 such that 4^n-m and n-m are relatively prime.
 * https://oeis.org/A214717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the smallest m > 0 such that gcd(4^n - m, n - m) = 1
 *            Algorithm: for each candidate m we only need (4^n mod d) where
 *            d=|n-m|, which is obtained with modular exponentiation. Then
 *            gcd((4^n - m) mod d, d) is computed using the small numbers d
 *            and the remainder. The first m that yields gcd = 1...
 * Range:     u8 n <= 368, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* n is at least 1, according to the OEIS offset. */
    for (idx_t m = 1; ; ++m) {
        idx_t diff = (m > n) ? m - n : n - m;
        if (diff == 0)                /* skip m == n */
            continue;
        seq_t d = (seq_t)diff;        /* |n - m| */

        /* r = 4^n mod d  */
        seq_t r = mod_pow((seq_t)4, (uint64_t)n, d);

        /* (4^n - m) mod d = (r - (m mod d)) mod d */
        seq_t mmod = (seq_t)(m % diff);
        seq_t rem = (r + d - mmod) % d;

        if (gcd(rem, d) == (seq_t)1)
            return (seq_t)m;
    }
}
