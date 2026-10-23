/*
 * A102801  Let f(n) be the minimal number of distinct nonzero tetrahedral
 *          numbers that add to n (or -1 if n is not a sum of distinct
 *          tetrahedral numbers); sequence gives numbers n for which f(n) = 2.
 * https://oeis.org/A102801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A102801
   Enumerates positive integers n that are representable as a sum of two
   distinct non‑zero tetrahedral numbers, but are not themselves tetrahedral.
   a(n) is the nth such integer (offset 1). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* Static state to keep the sequence between calls. */
    static bool initialized = false;
    static seq_t *terms = NULL;      /* array of sequence terms */
    static size_t terms_cnt = 0;     /* number of terms in 'terms' */
    static idx_t current_limit = 0;  /* current maximum number examined */

    if (!initialized) {
        current_limit = 1000;         /* start with a modest limit */
        initialized = true;
    }

    /* Grow the table until we have at least 'n' terms. */
    while (terms_cnt < n) {
        /* Double the search window (will grow quickly for large n). */
        if (current_limit < ((idx_t)1 << 30))
            current_limit *= 2;
        else
            current_limit += 100000;      /* fallback if really huge */

        /* ----------------------------------------------------------------
           Generate all tetrahedral numbers up to 'current_limit'.
           T(k) = k*(k+1)*(k+2)/6  (k ≥ 1)
          ---------------------------------------------------------------- */
        size_t cnt = 0;
        /* Upper bound estimate for k using integer arithmetic. */
        idx_t k = 1;
        while (true) {
            seq_t t = (seq_t)k * (seq_t)(k + 1) * (seq_t)(k + 2) / 6;
            if (t > current_limit) break;
            k++;
        }
        cnt = (size_t)(k - 1); /* number of positive tetrahedrons <= current_limit */

        seq_t *tetra = (seq_t *)malloc(cnt * sizeof(seq_t));
        for (size_t i = 0; i < cnt; ++i) {
            idx_t kk = (idx_t)(i + 1);
            tetra[i] = (seq_t)kk * (seq_t)(kk + 1) * (seq_t)(kk + 2) / 6;
        }

        /* ----------------------------------------------------------------
           Boolean arrays:  is_tetra[x]  → x itself is a tetrahedral number
                              rep[x]      → x is sum of two distinct tetrahedral numbers
          ---------------------------------------------------------------- */
        size_t cap = (size_t)current_limit + 1;
        uint8_t *is_tetra = (uint8_t *)calloc(cap, 1);
        uint8_t *rep      = (uint8_t *)calloc(cap, 1);

        for (size_t i = 0; i < cnt; ++i) {
            is_tetra[(size_t)tetra[i]] = 1;
        }

        /* Compute all pairwise sums. */
        for (size_t i = 0; i < cnt; ++i) {
            for (size_t j = i + 1; j < cnt; ++j) {
                seq_t s = tetra[i] + tetra[j];
                if (s <= current_limit)
                    rep[(size_t)s] = 1;
            }
        }

        /* ----------------------------------------------------------------
           Extract the sequence terms up to current_limit.
          ---------------------------------------------------------------- */
        size_t new_terms = 0;
        for (size_t v = 1; v <= current_limit; ++v) {
            if (rep[v] && !is_tetra[v])
                new_terms++;
        }

        seq_t *new_arr = (seq_t *)malloc(new_terms * sizeof(seq_t));
        size_t pos = 0;
        for (size_t v = 1; v <= current_limit; ++v) {
            if (rep[v] && !is_tetra[v])
                new_arr[pos++] = (seq_t)v;
        }

        /* Replace old arrays. */
        free(terms);
        terms = new_arr;
        terms_cnt = new_terms;

        /* Clean up temporary arrays. */
        free(tetra);
        free(is_tetra);
        free(rep);
    }

    /* Return the requested term (1‑based index). */
    return terms[n - 1];
}
