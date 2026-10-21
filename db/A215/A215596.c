/*
 * A215596  Expansion of psi(-x) * f(-x^4)^3 in powers of x where psi(), f()
 *          are Ramanujan theta functions.
 * https://oeis.org/A215596
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   A215596(n) is the coefficient of q^n in the product * ∏_{odd
 *            m≥1} (1 – q^m) · ∏_{k≥1} (1 – q^{4k})^4 * i.e. the expansion of
 *            ψ(-x)·f(-x^4)^3
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes the sequence term a(n) for any n ≥ 0.
 * It rebuilds the coefficient array from scratch up to n.
 * Complexity: O(n^2), fast enough for n ≤ ~10000. */

#include <stdlib.h>

static seq_t *coeff = NULL;   /* coefficient array a[0..N]   */
static size_t coeff_cap = 0;  /* allocated capacity          */

static void ensure_capacity(size_t need)
{
    if (coeff_cap < need) {
        seq_t *new_coeff = (seq_t *)realloc(coeff, need * sizeof(seq_t));
        coeff = new_coeff;
        coeff_cap = need;
    }
}

seq_t a(idx_t n)
{
    size_t N = (size_t)n;
    /* Allocate and clear a[0..N] */
    ensure_capacity(N + 1);
    for (size_t i = 0; i <= N; ++i) coeff[i] = 0;
    coeff[0] = 1;                /* constant term of the product */

    /* Apply each factor of the infinite product up to index N */
    for (idx_t f = 1; f <= n; ++f) {
        if (f & 1) {            /* odd factor: (1 – q^f) */
            for (long long i = (long long)N; i >= (long long)f; --i) {
                size_t ui = (size_t)i;
                coeff[ui] -= coeff[ui - (size_t)f];
            }
        } else if ((f & 3) == 0) {   /* f divisible by 4: (1 – q^f)^4 */
            for (long long i = (long long)N; i >= (long long)f; --i) {
                size_t ui = (size_t)i;
                seq_t val = coeff[ui];
                val -= (seq_t)4 * coeff[ui - (size_t)f];
                if (i >= 2 * f)
                    val += (seq_t)6 * coeff[ui - (size_t)(2 * f)];
                if (i >= 3 * f)
                    val -= (seq_t)4 * coeff[ui - (size_t)(3 * f)];
                if (i >= 4 * f)
                    val += coeff[ui - (size_t)(4 * f)];
                coeff[ui] = val;
            }
        }   /* even but not divisible by 4: factor is 1, skip */
    }

    return coeff[n];
}
