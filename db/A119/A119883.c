/*
 * A119883  Expansion of E.g.f. (1 + 2*x + x^2/2) * sech(x).
 * https://oeis.org/A119883
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = EulerE(n) + 2*n*EulerE(n-1) + n*(n-1)/2*EulerE(n-2) where
 *            EulerE(m) are the Euler (secant) numbers: E_{2k+1} = 0, E_{2k} =
 *            -∑_{j=0}^{k-1} C(2k,2j) * E_{2j} (E₀ = 1)
 * Range:     i8 n <= 6, i16 n <= 10, i32 n <= 14, i64 n <= 24, i128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache of Euler numbers at even indices:
   e[m] holds EulerE(2*m). Only even indices are stored. */
static seq_t *e = NULL;
static idx_t e_len = 0;

/* Ensure that e[m] is known for all 0 ≤ m ≤ m_needed.  */
static void ensure_even_euler(idx_t m_needed)
{
    if (m_needed < e_len)
        return;

    /* Grow the array to accommodate the new entries. */
    seq_t *new_e = (seq_t *)realloc(e, (m_needed + 1) * sizeof(seq_t));
    if (!new_e)
        return;                     /* allocation failed – sequence end‑of‑file */
    e = new_e;

    /* Compute new Euler numbers.  */
    for (idx_t m = e_len; m <= m_needed; ++m) {
        if (m == 0) {
            e[0] = (seq_t)1;         /* E₀ = 1 */
            continue;
        }

        seq_t sum   = (seq_t)0;
        seq_t binom = (seq_t)1;      /* C(2m,0) */

        for (idx_t k = 0; k < m; ++k) {
            sum += binom * e[k];

            if (k < m - 1) {
                /* Transition to C(2m,2(k+1)) :
                 *   C(2m,2k+2) = C(2m,2k) * (2m-2k)(2m-2k-1) / ((2k+2)(2k+1)) */
                seq_t num = (seq_t)(2 * m - 2 * k) *
                            (seq_t)(2 * m - 2 * k - 1);
                seq_t den = (seq_t)(2 * k + 2) *
                            (seq_t)(2 * k + 1);
                binom = binom * num / den;  /* exact division */
            }
        }

        e[m] = -sum;
    }

    e_len = m_needed + 1;
}

/* Public routine required by the build system.  */
seq_t a(idx_t n)
{
    /* Handle the first two terms explicitly.  */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    /* Determine the largest even index needed for Euler numbers.  */
    idx_t m_needed = n / 2;
    if (n >= 1) {
        if ((n - 1) % 2 == 0) {
            idx_t m = (n - 1) / 2;
            if (m > m_needed) m_needed = m;
        }
    }
    if (n >= 2) {
        if ((n - 2) % 2 == 0) {
            idx_t m = (n - 2) / 2;
            if (m > m_needed) m_needed = m;
        }
    }

    ensure_even_euler(m_needed);

    /* Retrieve the required Euler values (0 for odd indices).  */
    seq_t e_n  = (n % 2 == 0) ? e[n / 2] : (seq_t)0;
    seq_t e_n1 = ((n - 1) % 2 == 0) ? e[(n - 1) / 2] : (seq_t)0;
    seq_t e_n2 = ((n >= 2) && ((n - 2) % 2 == 0))
                 ? e[(n - 2) / 2]
                 : (seq_t)0;

    /* Compute a(n) using the closed form.  */
    seq_t term1 = e_n;
    seq_t term2 = (seq_t)2 * (seq_t)n * e_n1;
    /* Compute n*(n-1)/2 in a way that preserves exactness.  */
    seq_t t = (seq_t)n * (seq_t)(n - 1) / 2;
    seq_t term3 = t * e_n2;

    return term1 + term2 + term3;
}
