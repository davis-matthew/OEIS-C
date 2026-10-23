/*
 * A135578  Numbers that are not the sum of three distinct perfect powers
 *          (assuming 1, but not 0, is a perfect power).
 * https://oeis.org/A135578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = the nth integer that cannot be expressed as x + y + z
 *            with x, y, z distinct perfect powers (1, 2^2, 2^3, 3^2, ...)
 * Range:     u8 n <= 32, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes numbers that are not the sum of three distinct perfect powers
   (perfect powers include 1, exclude 0). Implements OEIS sequence A135578. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* Comparator for qsort on idx_t values */
static int cmp_idx(const void *a, const void *b)
{
    const idx_t *x = (const idx_t *)a;
    const idx_t *y = (const idx_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* Global state used by ensure_terms */
static seq_t *terms           = NULL;   /* array of sequence values            */
static size_t terms_len       = 0;     /* number of terms stored              */
static size_t terms_cap       = 0;     /* capacity of terms array             */
static idx_t  last_checked    = 0;     /* highest integer processed so far    */
static idx_t  current_limit   = 0;     /* current upper limit of processed numbers */

/* Ensure that the first n terms of the sequence are available */
static void ensure_terms(idx_t n)
{
    if (n <= terms_len) return;

    /* Grow the sequence until we have at least n terms */
    while (terms_len < (size_t)n) {
        /* Double the limit each iteration, but make sure it grows at least to 256 */
        idx_t new_limit = current_limit ? current_limit * 2 : 256;
        if (new_limit <= current_limit) /* overflow safeguard */
            new_limit = ~0u;

        /* Build list of perfect powers up to new_limit (1 is included). */
        size_t pp_cap = 16;
        idx_t *pp = (idx_t *)malloc(pp_cap * sizeof(idx_t));
        size_t pp_len = 0;

        /* include 1 as a perfect power */
        pp[pp_len++] = 1;

        idx_t max_base = (idx_t)floor(sqrt((double)new_limit));
        for (idx_t base = 2; base <= max_base; ++base) {
            unsigned __int128 v = (unsigned __int128)base * base;
            while (v <= new_limit) {
                if (pp_len >= pp_cap) {
                    pp_cap <<= 1;
                    pp = (idx_t *)realloc(pp, pp_cap * sizeof(idx_t));
                }
                pp[pp_len++] = (idx_t)v;
                v *= base;
            }
        }

        /* sort and uniquify */
        if (pp_len > 1) {
            qsort(pp, pp_len, sizeof(idx_t), cmp_idx);
            size_t uniq = 1;
            for (size_t i = 1; i < pp_len; ++i) {
                if (pp[i] != pp[i - 1]) {
                    pp[uniq++] = pp[i];
                }
            }
            pp_len = uniq;
        }

        /* Allocate a bitmap of representable numbers up to new_limit. */
        size_t rep_size = (size_t)new_limit + 1;
        char *rep = (char *)calloc(rep_size, 1);   /* zero‑initialised */

        /* Mark all numbers that CAN be expressed as the sum of three distinct perfect powers. */
        for (size_t i = 0; i < pp_len; ++i) {
            for (size_t j = i + 1; j < pp_len; ++j) {
                idx_t sum_ij = pp[i] + pp[j];
                if (j + 1 >= pp_len) continue;
                if (sum_ij + pp[j + 1] > new_limit) continue;  /* no possible k */
                for (size_t k = j + 1; k < pp_len; ++k) {
                    idx_t sum = sum_ij + pp[k];
                    if (sum > new_limit) break;               /* next k would be larger */
                    rep[(size_t)sum] = 1;
                }
            }
        }

        /* Append all numbers > last_checked that are NOT representable. */
        for (idx_t x = last_checked + 1; x <= new_limit; ++x) {
            if (!rep[(size_t)x]) {
                if (terms_len >= terms_cap) {
                    terms_cap = terms_cap ? terms_cap * 2 : 16;
                    terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
                }
                terms[terms_len++] = (seq_t)x;
            }
        }

        /* Clean up and prepare for the next iteration if needed. */
        free(rep);
        free(pp);
        last_checked = new_limit;
        current_limit = new_limit;
    }
}

/* Public function required by the problem statement. */
seq_t a(idx_t n)
{
    /* The sequence uses 1‑based indexing.  Ensure we have at least n terms. */
    ensure_terms(n);
    return terms[n - 1];
}
