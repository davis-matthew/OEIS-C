/*
 * A029582  E.g.f. sin(x) + cos(x) + tan(x).
 * https://oeis.org/A029582
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n even : (-1)^(n/2) n odd : T_n + (-1)^((n-1)/2) where
 *            T_n are tangent numbers: T_1 = 1; T_{2m+1} = Σ_{k=0}^{m-1}
 *            C(2m,2k+1) * T_{2k+1} * T_{2(m-1-k)+1} (m ≥ 1)
 * Range:     i8 n <= 6, i16 n <= 10, i32 n <= 16, i64 n <= 24, i128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */
#include <stdlib.h>   /* for malloc, realloc */

/* ---- tangent number cache ---------------------------------------- */
static seq_t *tan_cache = NULL;   /* tan_cache[m] = T_{2m+1}   */
static idx_t  tan_max  = (idx_t)-1; /* highest m computed (-1 if not yet) */

/* ensure tan_cache has size at least m_needed+1 and that all missing
   tangent numbers up to m_needed are computed. */
static void ensure_tangent_cache(idx_t m_needed)
{
    /* initial allocation */
    if (tan_cache == NULL) {
        tan_cache = (seq_t *)malloc(sizeof(seq_t));
        if (!tan_cache) return; /* allocation failed */
        tan_cache[0] = (seq_t)1; /* T_1 = 1 */
        tan_max = 0;
    }

    /* enlarge cache if necessary */
    if (m_needed > tan_max) {
        size_t new_cap  = (size_t)(m_needed + 1);
        seq_t *new_tbl = (seq_t *)realloc(tan_cache, new_cap * sizeof(seq_t));
        if (!new_tbl) return; /* reallocation failed */
        tan_cache = new_tbl;

        /* compute missing tangent numbers */
        for (idx_t m = tan_max + 1; m <= m_needed; ++m) {
            seq_t sum = 0;
            /* C(2m,1) */
            seq_t binom = (seq_t)(2 * m);

            for (idx_t k = 0; k < m; ++k) {
                seq_t term = tan_cache[k];
                term *= tan_cache[m - 1 - k];
                term *= binom;
                sum += term;

                if (k < m - 1) {
                    /* update binom to C(2m,2k+3) from C(2m,2k+1)          */
                    /* C(n,k+2) = C(n,k) * (n-k)*(n-k-1)/((k+1)*(k+2))    */
                    seq_t num1 = (seq_t)(2 * m - (2 * k + 1));
                    seq_t num2 = (seq_t)(2 * m - (2 * k + 2));
                    seq_t den1 = (seq_t)(2 * k + 2);
                    seq_t den2 = (seq_t)(2 * k + 3);
                    binom = binom * num1 / den1;
                    binom = binom * num2 / den2;
                }
            }
            tan_cache[m] = sum;
        }
        tan_max = m_needed;
    }
}

/* ---- main sequence function -------------------------------------- */
seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0) return (seq_t)1;

    /* even indices: a(2k) = (-1)^k */
    if ((n & 1u) == 0) {
        idx_t k = n >> 1;
        return ((k & 1u) == 0) ? (seq_t)1 : (seq_t)-1;
    }

    /* odd indices: a(2m+1) = T_{2m+1} + (-1)^m */
    idx_t m = (n - 1) >> 1;

    /* ensure tangent numbers up to m are available */
    ensure_tangent_cache(m);

    seq_t sign = ((m & 1u) == 0) ? (seq_t)1 : (seq_t)-1;
    return tan_cache[m] + sign;
}
