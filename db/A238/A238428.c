/*
 * A238428  Expansion of F(x) where F(x) = 1 + x / (1 - x * F(x) - x^2 *
 *          F(x^2) ).
 * https://oeis.org/A238428
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1. For n ≥ 1 define c(m) = a(m‑1) + (m even ? a[(m‑2)/2]
 *            : 0). Then a(n) = Σ_{k=0}^{n‑1} a(k)·c(n‑k) – c(n) + (n==1 ? 1 :
 *            0). This recurrence follows from the functional equation F(x) =
 *            1 + x / (1 – x F(x) – x² F(x²)) and yields the OEIS sequence
 *            A238428. The computation uses only integer...
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 22, u64 n <= 42, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *val = NULL;
    static seq_t *c   = NULL;
    static size_t cap = 0;

    /* ensure capacity for indices up to n */
    if (n >= cap) {
        size_t new_cap = cap ? cap : 1;
        while (new_cap <= n) new_cap <<= 1;

        seq_t *tmp_val = (seq_t *)realloc(val, new_cap * sizeof(seq_t));
        if (!tmp_val) return (seq_t)0;          /* allocation failure */
        val = tmp_val;

        seq_t *tmp_c = (seq_t *)realloc(c, new_cap * sizeof(seq_t));
        if (!tmp_c) return (seq_t)0;            /* allocation failure */
        c = tmp_c;

        /* initialise the newly allocated part to zero */
        if (new_cap > cap) {
            size_t i;
            for (i = cap; i < new_cap; ++i) {
                val[i] = (seq_t)0;
                c[i]   = (seq_t)0;
            }
        }
        cap = new_cap;
    }

    /* initialise base values if not yet set */
    if (cap > 0 && val[0] == (seq_t)0) {
        val[0] = (seq_t)1;
        c[0]   = (seq_t)0;            /* unused, but keep zero */
    }

    /* compute values up to n */
    for (idx_t m = 1; m <= n; ++m) {
        /* compute c(m) = a(m-1) + (m even ? a[(m-2)/2] : 0) */
        seq_t cm = val[m-1];
        if ((m & 1) == 0)          /* even */
            cm += val[(m-2)/2];
        c[m] = cm;

        /* sum_{k=0}^{m-1} a(k) * c(m-k) */
        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k < m; ++k)
            sum += val[k] * c[m-k];

        /* a(m) = sum - c(m) + (m==1 ? 1 : 0) */
        seq_t term = sum - cm;
        if (m == 1) term += (seq_t)1;
        val[m] = term;
    }

    return val[n];
}
