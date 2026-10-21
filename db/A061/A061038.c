/*
 * A061038  Denominator of 1/4 - 1/n^2.
 * https://oeis.org/A061038
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, frac, nice, easy
 * Formula:   a(n) = 4*n^2 / gcd(4*n^2, n^2-4)
 * Range:     u8 n <= 7, u16 n <= 127, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the gcd utility from utils/gcd.h */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* The function is defined for n >= 2. */
    seq_t ns = (seq_t)n;          /* cast index to seq_t */
    seq_t n2 = ns * ns;           /* n^2 */
    seq_t num = (seq_t)4 * n2;    /* 4*n^2 */
    seq_t den = n2 - (seq_t)4;    /* n^2 - 4, works even for n=2 (den=0) */
    seq_t g = gcd(num, den);      /* gcd(4*n^2, n^2-4) */
    return num / g;               /* integer division gives the denominator */
}
