/*
 * A119345  Numbers having exactly one representation as sum of two triangular
 *          numbers.
 * https://oeis.org/A119345
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers having exactly one representation as sum of two triangular numbers.
   This function generates the terms by counting representations up to the
   largest value needed, and it remembers the previously computed terms
   to answer any call in O(1) after an initial O(n²) build. */
#include <stdlib.h>
#include <stdint.h>

/* Global cached data */
static seq_t *terms = NULL;           /* array of terms sorted increasingly   */
static size_t term_size = 0;          /* number of terms in the array          */
static idx_t  cached_max = 0;         /* largest value that has been counted   */

/* Recompute the list of terms up to a given maximum value.  The function
   frees and reallocates the terms array.  It is O(k²) where k is the
   number of triangular numbers not exceeding max_val. */
static void recompute_terms_up_to(idx_t max_val)
{
    /* Determine number of triangular numbers ≤ max_val */
    idx_t kmax = 0;
    while ((idx_t)kmax * (kmax + 1) / 2 <= max_val) {
        /* Careful with overflow: cast to seq_t, then back to idx_t */
        seq_t t = (seq_t)kmax * (kmax + 1) / 2;
        if (t > (seq_t)max_val) break;
        ++kmax;
    }
    /* Allocate triangular array */
    idx_t *tri = (idx_t *)malloc(kmax * sizeof(idx_t));
    for (idx_t i = 0; i < kmax; ++i) {
        tri[i] = (idx_t)i * (i + 1) / 2;
    }

    /* Count representations */
    uint16_t *cnt = (uint16_t *)calloc((size_t)max_val + 1, sizeof(uint16_t));
    for (idx_t i = 0; i < kmax; ++i) {
        idx_t Ti = tri[i];
        for (idx_t j = i; j < kmax; ++j) {
            idx_t s = Ti + tri[j];
            if (s > max_val) break;
            ++cnt[s];
        }
    }

    /* Count how many numbers have exactly one representation */
    size_t new_term_count = 0;
    for (idx_t v = 0; v <= max_val; ++v) {
        if (cnt[v] == 1) ++new_term_count;
    }

    /* Allocate new terms array */
    seq_t *new_terms = (seq_t *)malloc(new_term_count * sizeof(seq_t));
    size_t pos = 0;
    for (idx_t v = 0; v <= max_val; ++v) {
        if (cnt[v] == 1) {
            /* Value v fits into seq_t for all tested indices. */
            new_terms[pos++] = (seq_t)v;
        }
    }

    /* Free old structures */
    free(cnt);
    free(tri);
    free(terms);

    /* Store new data */
    terms      = new_terms;
    term_size  = new_term_count;
    cached_max = max_val;
}

/* Ensure that at least n terms are available.  If not, increase the
   cached maximum and recompute. */
static void ensure_terms(idx_t n)
{
    if (n <= term_size) return;
    /* Start with a reasonable maximum and double until we get enough terms */
    idx_t grow = (cached_max == 0) ? 100 : cached_max * 2;
    while (term_size < n) {
        recompute_terms_up_to(grow);
        grow *= 2;
    }
}

/* External linkage as required by the build system */
seq_t a(idx_t n)
{
    /* n is 1‑based */
    if (n == 0) return 0;               /* defensive, not needed for valid input */
    ensure_terms(n);
    return terms[n - 1];
}
