/*
 * A202145  G.f.: 1 + Sum_{n>=1} x^n * Product_{k=1..n} (1 - x^k) / (1 -
 *          x^(2*k+1)).
 * https://oeis.org/A202145
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in * 1 + Sum_{k>=1} x^k *
 *            Prod_{i=1..k} (1 - x^i) / (1 - x^(2*i+1)) * This implementation
 *            builds the generating function incrementally and * caches all
 *            computed terms. The algorithm runs in O(n²) time and * O(n)
 *            memory, which is adequate for n up to at least 10 000
 * Range:     i8 n <= 500, i16 n <= 500, i32 n <= 500, i64 n <= 500,
 *            i128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq_cache = NULL;   /* cached values a(0)..a(n)   */
static size_t cache_len   = 0;    /* current number of cached entries */

/* Ensure that seq_cache contains a(0)…a(n). */
static void ensure_cache(idx_t n)
{
    /* Determine the required array size.  */
    size_t needed = (size_t)n + 1;
    if (cache_len >= needed)
        return;                  /* already cached up to n */

    /* Allocate temporary arrays; use casts so that the code compiles
     * both in C and in C++ when seq_t is an arbitrary‑precision class. */
    seq_t *B   = (seq_t*)calloc(needed, sizeof(*B));   /* holds current product   */
    seq_t *seq = (seq_t*)calloc(needed, sizeof(*seq)); /* holds the result array */

    if (!B || !seq) {            /* allocation failed */
        free(B);
        free(seq);
        exit(EXIT_FAILURE);
    }

    /* Initial conditions: G(x) starts with 1.  */
    seq[0] = 1;
    B[0]   = 1;

    /* Build the sequence up to n.  */
    for (idx_t curr = 1; curr <= n; ++curr)
    {
        /* Multiply B by (1 - x^curr).  */
        for (idx_t i = n; i >= curr; --i)
            B[i] -= B[i - curr];

        /* Multiply B by 1/(1 - x^(2*curr+1)) -> add geometric series.  */
        idx_t d = 2 * curr + 1;
        for (idx_t i = d; i <= n; ++i)
            B[i] += B[i - d];

        /* Add the shifted product x^curr * B to the result array.  */
        for (idx_t i = 0; i + curr <= n; ++i)
            seq[curr + i] += B[i];
    }

    /* Replace the old cache.  */
    free(seq_cache);
    seq_cache = seq;
    cache_len = needed;

    /* Temporary product array can be freed.  */
    free(B);
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return seq_cache[n];
}
