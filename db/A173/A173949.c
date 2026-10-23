/*
 * A173949  a(n) = numerator of (Zeta(2, 1/4) - Zeta(2, n+1/4))/16, where Zeta
 *          is the Hurwitz Zeta function.
 * https://oeis.org/A173949
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = numerator of (Zeta(2,1/4) - Zeta(2,n+1/4)) / 16 ≡
 *            numerator of ∑_{k=0}^{n-1} 1/(4k+1)^2 a(0) = 0, a(1) = 1, a(2) =
 *            26, … (OEIS A173949)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 18,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"   /* gcd(seq_t, seq_t) is available */

/* Return the nth term of the sequence. */
seq_t a(idx_t n)
{
    /* trivial case */
    if (n == 0) return 0;

    seq_t p = 0;          /* current numerator   */
    seq_t q = 1;          /* current denominator */

    for (idx_t k = 0; k < n; ++k)
    {
        /* base = 4*k + 1,  term = 1 / (base)^2 */
        seq_t base = (seq_t)4 * k + 1;
        seq_t term_den = base * base;          /* (4k+1)^2 */

        /* add the term 1/term_den to p/q:
           new_p = p*term_den + q
           new_q = q*term_den
           reduce to lowest terms       */
        seq_t new_p = p * term_den + q;
        seq_t new_q = q * term_den;

        seq_t g = gcd(new_p, new_q);
        p = new_p / g;
        q = new_q / g;
    }

    return p;   /* numerator of a(n) */
}
