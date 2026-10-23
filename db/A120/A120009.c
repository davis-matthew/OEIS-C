/*
 * A120009  G.f.: A(x) = (x-x^2) o x/(1-x) o (1-sqrt(1-4*x))/2, a composition
 *          of functions involving the Catalan function and its inverse.
 * https://oeis.org/A120009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 3*C(n) - C(n+1), where C(m) is the m‑th Catalan number.
 *            (Offset 1 – a(1)=1, a(2)=1, …)
 * Range:     i8 n <= 3, i16 n <= 7, i32 n <= 15, i64 n <= 27, i128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* If called with n < 1 the behaviour is undefined, but we return 0
       defensively to avoid a crash. */
    if (n == 0)
        return 0;

    /* Catalan numbers are generated iteratively.
       C(0) = 1.
       C(k+1) = C(k) * (4*k + 2) / (k + 2).         */
    seq_t cat      = 1;   /* C(0) */
    seq_t cat_n    = 0;   /* will hold C(n)   */
    seq_t cat_n1   = 0;   /* will hold C(n+1) */

    for (idx_t i = 0; i <= n; ++i) {
        /* Before updating cat we may need its current value C(i). */
        if (i == n)
            cat_n = cat;     /* cat == C(n) */

        /* Compute the next Catalan number. */
        seq_t num = cat * ((seq_t)(4 * i + 2));
        seq_t den = ((seq_t)(i + 2));
        cat = num / den;    /* cat now equals C(i+1) */
    }

    cat_n1 = cat;           /* after the loop C(n+1) has been computed */

    return 3 * cat_n - cat_n1;
}
