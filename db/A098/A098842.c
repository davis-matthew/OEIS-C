/*
 * A098842  Number of n-digit Fibonacci numbers.
 * https://oeis.org/A098842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = number of Fibonacci numbers having exactly n decimal
 *            digits (with a(1)=7 including F0). For n≥2 the count is given by
 *            a(n) = floor((n + log10(√5)) / log10(φ)) –
 *            ceil(((n‑1)+log10(√5)) / log10(φ)) + 1, where φ = (1+√5)/2. The
 *            formula is derived from the Binet approximation for Fk and
 *            holds...
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* needs C11/C++17 compatible math functions */
#include <math.h>

/* external integer types supplied by the build */
extern seq_t a(idx_t n)
{
    /* handle the special first term */
    if (n == 0)
        return (seq_t)0;      /* offset not used */
    if (n == 1)
        return (seq_t)7;      /* 0,1,1,2,3,5,8 */

    /* pre–computed constants as long double */
    const long double sqrt5 = sqrtl(5.0L);
    const long double phi   = (1.0L + sqrt5) / 2.0L;
    const long double log10_phi   = log10l(phi);
    const long double log10_sqrt5 = log10l(sqrt5);

    /* compute the integer bounds for the index k of Fibonacci numbers */
    /* lowest k with n digits */
    idx_t k_min = (idx_t)ceill( ((long double)(n - 1) + log10_sqrt5) / log10_phi );
    /* highest k with n digits */
    idx_t k_max = (idx_t)floorl( ((long double)n + log10_sqrt5) / log10_phi );

    if (k_max < k_min)
        return (seq_t)0;  /* no Fibonacci number has exactly n digits */

    return (seq_t)(k_max - k_min + 1);
}
