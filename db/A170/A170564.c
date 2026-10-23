/*
 * A170564  Number of reduced words of length n in Coxeter group on 27
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^47 = I.
 * https://oeis.org/A170564
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A170564
   a(0) = 1
   For 1 ≤ n ≤ 46 :  a(n) = 27 · 26^(n‑1)
   For n ≥ 47     :  a(n) = 25 · (a(n‑1)+⋯+a(n‑46)) – 325 · a(n‑47)   */
/* Note: a(n) is computed only for n ≥ 0; values beyond the known offset
   are derived from the linear recurrence derived from the generating
   function.                                          */

#include <stdlib.h>

/* External integer types supplied by the build */
extern seq_t a(idx_t n);

/* Static cache to store computed terms */
static seq_t *cache = NULL;          /* allocated array of a(0)…a(max_computed)   */
static idx_t  max_computed = (idx_t)-1; /* (idx_t)-1 denotes that no term was cached yet */

/* Ensure that cache holds values up to index n (inclusive) */
static void ensure_cached(idx_t n)
{
    if (max_computed != (idx_t)-1 && n <= max_computed)
        return;                      /* nothing to do */

    /* Allocate or enlarge the cache */
    size_t new_size = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp)
        abort();                     /* allocation failed */
    cache = tmp;

    /* First time: initialise a(0) and compute up to n */
    if (max_computed == (idx_t)-1) {
        cache[0] = (seq_t)1;
        max_computed = 0;
        /* Compute initial values 1…n */
        for (idx_t i = 1; i <= n; ++i) {
            if (i <= 46) {
                if (i == 1)
                    cache[i] = (seq_t)27;
                else
                    cache[i] = cache[i-1] * (seq_t)26;
            } else {
                /* recurrence for i ≥ 47 */
                seq_t sum = (seq_t)0;
                for (idx_t j = 1; j <= 46; ++j)
                    sum += cache[i-j];
                cache[i] = ((seq_t)25 * sum) - ((seq_t)325 * cache[i-47]);
            }
        }
        return;
    }

    /* Cache already has values up to max_computed, extend to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i <= 46) {
            if (i == 1)
                cache[i] = (seq_t)27;
            else
                cache[i] = cache[i-1] * (seq_t)26;
        } else {
            seq_t sum = (seq_t)0;
            for (idx_t j = 1; j <= 46; ++j)
                sum += cache[i-j];
            cache[i] = ((seq_t)25 * sum) - ((seq_t)325 * cache[i-47]);
        }
    }
    max_computed = n;
}

/* Public function required by the test harness */
seq_t
a(idx_t n)
{
    ensure_cached(n);
    return cache[n];
}
