/*
 * A010793  a(n) = n!*(n+4)! / 4!.
 * https://oeis.org/A010793
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * (n+4)! / 4! (4! = 24)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 19,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t res = 1;          /* running product */
    seq_t rem = 24;         /* remaining factor of 24 to cancel */

    /* multiply by 1..n twice (i.e. n! twice) */
    for (idx_t k = 1; k <= n; ++k) {
        seq_t g = gcd(rem, (seq_t)k);
        rem /= g;
        res *= (seq_t)k / g;
    }
    for (idx_t k = 1; k <= n; ++k) {
        seq_t g = gcd(rem, (seq_t)k);
        rem /= g;
        res *= (seq_t)k / g;
    }

    /* multiply by n+1 .. n+4 (the extra part of (n+4)! ) */
    for (idx_t k = n + 1; k <= n + 4; ++k) {
        seq_t g = gcd(rem, (seq_t)k);
        rem /= g;
        res *= (seq_t)k / g;
    }

    /* any remaining factor of 24 (should be 1) */
    if (rem != (seq_t)1)
        res /= rem;

    return res;
}
