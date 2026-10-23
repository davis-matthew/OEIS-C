/*
 * A014280  Number of trees on n nodes with 3 forbidden limbs of size 4, 5 and
 *          6.
 * https://oeis.org/A014280
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 23, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(0) = 0, a(1) = 1, a(2) = 1, a(3) = 2,
 * a(n) = g(n‑1) – g(n‑4) – g(n‑5) – g(n‑6)
 *        + g(n‑8) + g(n‑9) + g(n‑10) – g(n‑13),  n ≥ 4
 * where g(0) = 1 and
 * g(n) = (1/n) * Σ_{k=1..n} ( Σ_{d|k} d·a(d) ) · g(n‑k).
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n) {
    static seq_t *a_tab = NULL;
    static seq_t *g_tab = NULL;
    static seq_t *s_tab = NULL;
    static idx_t cap   = 0;   /* allocated capacity (number of slots) */
    static idx_t max_i = 0;   /* highest index computed so far */

    /* enlarge arrays if needed */
    if ((idx_t)(n + 1) > cap) {
        idx_t new_cap = n + 1;
        seq_t *na = (seq_t *)realloc(a_tab, new_cap * sizeof(seq_t));
        seq_t *ng = (seq_t *)realloc(g_tab, new_cap * sizeof(seq_t));
        seq_t *ns = (seq_t *)realloc(s_tab, new_cap * sizeof(seq_t));
        if (!na || !ng || !ns) { exit(EXIT_FAILURE); }

        /* zero any newly allocated slots */
        for (idx_t i = cap; i < new_cap; ++i) {
            na[i] = 0;
            ng[i] = 0;
            ns[i] = 0;
        }
        a_tab = na;
        g_tab = ng;
        s_tab = ns;
        cap   = new_cap;
    }

    /* initialise base case if the tables are freshly allocated */
    if (cap == 1) {
        a_tab[0] = 0;
        g_tab[0] = 1;
        s_tab[0] = 0;
        max_i   = 0;
    }

    /* compute values from (max_i + 1) up to n */
    for (idx_t i = max_i + 1; i <= n; ++i) {
        /* ---- compute a[i] ---- */
        seq_t pos = 0, neg = 0;
        if (i >= 1) pos += g_tab[i - 1];
        if (i >= 8) pos += g_tab[i - 8];
        if (i >= 9) pos += g_tab[i - 9];
        if (i >=10) pos += g_tab[i -10];
        if (i >= 4) neg += g_tab[i - 4];
        if (i >= 5) neg += g_tab[i - 5];
        if (i >= 6) neg += g_tab[i - 6];
        if (i >=13) neg += g_tab[i -13];
        a_tab[i] = pos - neg;   /* guaranteed non‑negative */

        /* ---- compute s[i] = Σ_{d|i} d·a[d] ---- */
        seq_t s_val = 0;
        for (idx_t d = 1; d * d <= i; ++d) {
            if (i % d == 0) {
                s_val += ((seq_t)d) * a_tab[d];
                idx_t d2 = i / d;
                if (d2 != d) s_val += ((seq_t)d2) * a_tab[d2];
            }
        }
        s_tab[i] = s_val;

        /* ---- compute g[i] = (1/i) * Σ_{k=1..i} s[k]·g[i‑k] ---- */
        seq_t g_sum = 0;
        for (idx_t k = 1; k <= i; ++k) {
            g_sum += s_tab[k] * g_tab[i - k];
        }
        g_tab[i] = g_sum / (seq_t)i;   /* exact division by construction */
    }

    max_i = n;
    return a_tab[n];
}
