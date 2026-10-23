/*
 * A137430  Triangular sequence from coefficients of a cumulative sum of
 *          Chebyshev T(x,n) polynomials (A053120): p(x,n)=p(x,n-1)+T(x,n).
 * https://oeis.org/A137430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign
 * Formula:   a(n) is the coefficient of xᵖ in the polynomial p(x,deg) =
 *            Σ₀^deg T(x,k) (Chebyshev T of first kind), where deg = deg(p) =
 *            row k – 1 and p = n – (k(k – 1)/2) – 1. The sequence is the
 *            flattened triangle of these coefficients
 * Range:     i8 n <= 42, i16 n <= 66, i32 n <= 66, i64 n <= 66, i128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#define MAX_DEG 200   /* sufficient for n ≤ 10000 (deg ≈ √(2n)) */

static seq_t Tcoeff[MAX_DEG][MAX_DEG]; /* Chebyshev coefficients */
static seq_t pcoeff[MAX_DEG][MAX_DEG]; /* cumulative sums */
static idx_t built_deg = (idx_t)(-1);  /* highest degree built, −1 = none */

/* Return a(n) for n ≥ 1 (offset 1). Behaviour for n < 1 is undefined. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* guard, actual values start at n = 1 */

    /* Determine row k (1‑based) containing the n‑th term (degree k – 1). */
    idx_t k = 1;
    while (k * (k + 1) / 2 < n) ++k;
    idx_t degree = k - 1;                        /* polynomial degree    */
    idx_t terms_before = (k - 1) * k / 2;        /* terms before row k  */
    idx_t pos = n - terms_before - 1;            /* 0‑based position   */

    /* Build Chebyshev and cumulative arrays up to needed degree. */
    if (built_deg == (idx_t)(-1))
    {
        /* Base case | T_0(x) = 1, p(x,0) = 1 */
        Tcoeff[0][0] = 1;
        pcoeff[0][0] = 1;
        built_deg = 0;
    }

    for (idx_t d = built_deg + 1; d <= degree; ++d)
    {
        /* Compute Tcoeff[d][*] (degree d).  d ≥ 1 */
        if (d == 1)
        {
            Tcoeff[1][0] = 0;
            Tcoeff[1][1] = 1;
        }
        else
        {
            for (idx_t t = 0; t <= d; ++t)
            {
                seq_t term1 = 0;
                if (t > 0)
                    term1 = Tcoeff[d - 1][t - 1] + Tcoeff[d - 1][t - 1];   /* 2 * coefficient */
                seq_t term2 = (t <= d - 2) ? Tcoeff[d - 2][t] : 0;
                Tcoeff[d][t] = term1 - term2;
            }
        }

        /* Compute pcoeff[d][*] = pcoeff[d‑1][*] + Tcoeff[d][*] */
        for (idx_t t = 0; t <= d; ++t)
        {
            seq_t prev = (t <= d - 1) ? pcoeff[d - 1][t] : 0;
            pcoeff[d][t] = prev + Tcoeff[d][t];
        }
    }

    built_deg = degree;
    return pcoeff[degree][pos];
}
