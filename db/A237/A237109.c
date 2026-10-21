/*
 * A237109  a(n) is the numerator of 2*n / ((n+2) * (n+3)).
 * https://oeis.org/A237109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, mult
 * Formula:   a(n) = numerator of 2*n / ((n+2)*(n+3)) = 2*n / gcd(2*n,
 *            (n+2)*(n+3))
 * Range:     u8 n <= 14, u16 n <= 401, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* offset 1 – guard for n=0 */
    seq_t num   = (seq_t)n * 2ULL;            /* 2*n */
    seq_t denom = ((seq_t)n + 2ULL) * ((seq_t)n + 3ULL); /* (n+2)(n+3) */
    seq_t g     = gcd(num, denom);            /* common divisor */
    return num / g;                           /* numerator after reduction */
}
