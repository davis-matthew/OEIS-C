/*
 * A053696  Numbers that can be represented as a string of three or more 1's
 *          in a base >= 2.
 * https://oeis.org/A053696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 328, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: compute numbers that can be represented as a string of three or more 1's in some
   base ≤ ≥2 (OEIS A053696).  The routine generates all repunits up to an
   automatically chosen limit, sorts them, removes duplicates, caches the
   sorted list, and returns the n‑th element on demand. */

#include <stdlib.h>
#include <stdbool.h>

/* Auxiliary comparator for qsort on seq_t. */
static int seq_cmp(const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

/* Global cache of the sequence. */
static seq_t *seq_arr = NULL;   /* sorted, deduplicated values */
static size_t seq_len = 0;       /* number of entries in the cache */

/* Regenerate the whole cache so that seq_len ≥ target_n. */
static void regenerate_cache(idx_t target_n)
{
    /* Estimate a value high enough to contain at least target_n terms.
       Sequence grows approximately as n², so use a quadratic bound with a safety margin. */
    uint64_t lim64 = ((uint64_t)target_n + 100) * ((uint64_t)target_n + 100);
    if (lim64 < 1000) lim64 = 1000;
    seq_t limit = (seq_t)lim64;    /* cast to seq_t; for tiny seq_t types the bound will be small
                                      but enough for the required terms. */

    /* Temporary array for candidates. */
    size_t cap = 20000;
    seq_t *cand = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!cand) return;  /* allocation failure – just exit, leaving cache unchanged */
    size_t cand_len = 0;

    /* Enumerate repunits: length k ≥ 3, base b ≥ 2. */
    for (idx_t k = 3; ; ++k)
    {
        /* Minimal repunit with base 2: r_min = 1 + 2 + ... + 2^(k‑1) */
        seq_t r_min = 1;
        for (idx_t t = 1; t < k; ++t) {
            if (r_min > limit / 2) { r_min = limit + 1; break; }
            r_min = r_min * (seq_t)2 + 1;
        }
        if (r_min > limit) break;   /* larger k will only increase r_min */

        for (seq_t b = 2; ; ++b)
        {
            seq_t r = 1;
            for (idx_t t = 1; t < k; ++t) {
                if (r > limit / b) { r = limit + 1; break; }
                r = r * b + 1;
            }
            if (r > limit) break;   /* larger base will only increase r */

            /* Add to the list, resizing if necessary. */
            if (cand_len == cap) {
                cap *= 2;
                seq_t *tmp = (seq_t *)realloc(cand, cap * sizeof(seq_t));
                if (!tmp) { free(cand); return; } /* allocation failure */
                cand = tmp;
            }
            cand[cand_len++] = r;
        }
    }

    /* Sort candidates. */
    qsort(cand, cand_len, sizeof(seq_t), seq_cmp);

    /* Remove duplicates in place. */
    size_t uniq = 0;
    if (cand_len > 0) {
        seq_t prev = cand[0];
        cand[uniq++] = prev;
        for (size_t i = 1; i < cand_len; ++i) {
            if (cand[i] != prev) {
                prev = cand[i];
                cand[uniq++] = prev;
            }
        }
    }

    /* Replace old cache. */
    free(seq_arr);
    seq_arr = cand;
    seq_len = uniq;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset is 1; guard against zero */
    if (seq_arr == NULL || seq_len < (size_t)n)
        regenerate_cache(n);        /* ensure at least n entries */
    return seq_arr[n - 1];
}
