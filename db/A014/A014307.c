/*
 * A014307  Expansion of the e.g.f. sqrt(exp(x) / (2 - exp(x))).
 * https://oeis.org/A014307
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Σ_{j=1}^{n-1} (C(n,j)-1)·a(j), a(0)=1 (This is
 *            equivalent to a(n+1) = 1 + Σ_{j=1}^n (C(n+1,j)-1)·a(j).)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Cache-based O(n²) implementation. Handles any order of calls. */

#include <stdlib.h>
#include <stddef.h>

/* Global cache ---------------------------------------------------------- */
static seq_t *a_cache = NULL;  /* computed terms a[0..] */
static size_t a_cap   = 0;     /* allocated size of a_cache */
static idx_t  a_done  = 0;     /* largest index already computed */

/* Ensure the cache can hold indices up to n. */
static void ensure_cache(idx_t n)
{
    if ((idx_t)a_cap > n) return;          /* already large enough */

    size_t new_cap = a_cap ? a_cap << 1 : 4; /* start with 4 or double */
    while ((idx_t)new_cap <= n) new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!tmp) { abort(); }                  /* unrecoverable allocation failure */
    a_cache = tmp;
    a_cap   = new_cap;
}

/* Main entry point ------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Initialise cache on first call. */
    if (!a_cache) {
        a_cache = (seq_t *)malloc(sizeof(seq_t));
        if (!a_cache) { abort(); }
        a_cache[0] = (seq_t)1;     /* a(0) = 1 */
        a_cap   = 1;
        a_done  = 0;
    }

    /* Grow cache if needed. */
    ensure_cache(n);

    /* Return already computed value. */
    if (n <= a_done) return a_cache[n];

    /* Compute missing values up to n. */
    for (idx_t i = a_done + 1; i <= n; ++i) {
        /* Recurrence: a(i) = 1 + Σ_{j=1}^{i-1} (C(i,j)-1)·a(j) */
        seq_t sum = (seq_t)0;
        seq_t binom = (seq_t)i;   /* C(i,1) */

        for (idx_t j = 1; j <= i - 1; ++j) {
            sum += (binom - (seq_t)1) * a_cache[j];

            /* Update binom to C(i, j+1).  Division is exact. */
            if (j < i - 1) {
                seq_t numerator   = binom * (seq_t)(i - j);
                seq_t denominator = (seq_t)(j + 1);
                binom = numerator / denominator;
            }
        }

        a_cache[i] = sum + (seq_t)1;
    }

    a_done = n;
    return a_cache[n];
}
