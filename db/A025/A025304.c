/*
 * A025304  Numbers that are the sum of 2 distinct nonzero squares in exactly
 *          3 ways.
 * https://oeis.org/A025304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 228, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A025304: numbers that are the sum of 2 distinct nonzero squares in exactly 3 ways */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Helpers for allocation that work in both C and C++ */
#ifdef __cplusplus
static inline void free_terms(seq_t *p) { delete[] p; }
static inline seq_t *alloc_terms(size_t n) { return new seq_t[n]; }
#else
static inline void free_terms(seq_t *p) { free(p); }
static inline seq_t *alloc_terms(size_t n) { return (seq_t*)malloc(n * sizeof(seq_t)); }
#endif

/* Cache of discovered terms */
static seq_t *terms = NULL;          /* array of terms */
static size_t terms_size = 0;        /* number of terms stored */
static idx_t max_limit = 0;          /* largest number processed for counts */

/* Rebuild the cache up to a given limit */
static void recompute_terms(idx_t limit)
{
    /* Count how many times each number up to limit can be written as a^2+b^2, a<b */
    uint8_t *cnt = (uint8_t*)calloc((size_t)(limit + 1), sizeof(uint8_t));
    if (!cnt) return;  /* allocation failed – caller will not get more terms */

    for (idx_t a = 1; (seq_t)a * a <= (seq_t)limit; ++a) {
        seq_t a2 = (seq_t)a * a;
        for (idx_t b = a + 1; ; ++b) {
            seq_t n = a2 + (seq_t)b * b;
            if (n > (seq_t)limit) break;
            if (cnt[(size_t)n] < 255) ++cnt[(size_t)n];
        }
    }

    /* Count how many terms match the criterion */
    size_t tcount = 0;
    for (idx_t n = 1; n <= limit; ++n)
        if (cnt[(size_t)n] == 3) ++tcount;

    /* Allocate and fill the terms array */
    seq_t *tmp = alloc_terms(tcount);
    if (!tmp) {
        free(cnt);
        return;  /* allocation failed – leave cache unchanged */
    }

    size_t idx = 0;
    for (idx_t n = 1; n <= limit; ++n) {
        if (cnt[(size_t)n] == 3) tmp[idx++] = (seq_t)n;
    }

    free(cnt);

    /* Replace old cache */
    if (terms) free_terms(terms);
    terms = tmp;
    terms_size = tcount;
    max_limit = limit;
}

/* Ensure at least n terms are cached */
static void ensure_terms_size(idx_t n)
{
    if (n == 0) return;
    while (terms_size < (size_t)n) {
        /* Heuristically pick a limit that should produce enough terms */
        idx_t limit = max_limit ? max_limit * 2 : 1000;
        idx_t min_limit = n * 500;          /* rough density estimate */
        if (limit < min_limit) limit = min_limit;
        recompute_terms(limit);
        /* If still not enough, loop again with a larger limit */
    }
}

/* Public function required by the test harness */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined offset – harmless value */
    ensure_terms_size(n);
    /* terms is guaranteed to have at least n entries */
    return terms[n - 1];
}
