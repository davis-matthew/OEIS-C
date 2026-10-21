/*
 * A232625  Denominators of abs(n-8)/(2*n), n >= 1.
 * https://oeis.org/A232625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n / gcd((n-8) mod 16, 16)
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A232625: Denominators of |n-8|/(2*n),  n >= 1 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Compute (n-8) mod 16 in the range [0,15] */
    seq_t r = (seq_t)(n % 16);          /* n mod 16  (0..15) */
    seq_t rem = (r + 8) % 16;           /* (n-8) mod 16 */

    /* gcd of rem and 16 */
    seq_t d = gcd(rem, (seq_t)16);

    /* result = 2*n / d */
    return ((seq_t)2 * n) / d;
}
