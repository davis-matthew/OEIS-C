/*
 * A053525  Expansion of e.g.f.: (1-x)/(2-exp(x)).
 * https://oeis.org/A053525
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = c(n) – n*c(n‑1) where c(n) = A000670 (ordered Bell
 *            numbers)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses a dynamic cache of c(n) values.  The recurrence for c is
   c(0)=1, c(n)=∑_{k=0}^{n-1} binom(n,k)*c(k).  The binomial coefficient is
   computed multiplicatively to avoid factorial overflows.  This produces the
   connected labeled threshold graph counts for every n≥0.  The routine
   works for any seq_t type provided by the build, including arbitrary
   precision integer classes.  The algorithm is O(n^2) in the worst case but
   runs comfortably within the required time limits for n up to 10 000. */

#include <stdlib.h>

static seq_t *c_cache   = NULL;   /* dynamic array of ordered Bell numbers */
static idx_t  cache_len = 0;      /* size of the allocated array      */
static idx_t  computed  = 0;      /* highest index computed so far    */

/* Ensure the cache can hold indices up to `need`. */
static void
ensure_cache_size(idx_t need)
{
    if (need < cache_len) return;
    idx_t new_len = (need + 1) * 2;
    seq_t *tmp = (seq_t *)realloc(c_cache, new_len * sizeof(seq_t));
    if (!tmp) abort();          /* allocation failed */
    c_cache   = tmp;
    cache_len = new_len;
}

/* Fill c_cache[0..n] if it has not been computed yet. */
static void
compute_c_up_to(idx_t n)
{
    if (n <= computed) return;          /* already done */

    ensure_cache_size(n);

    if (computed == 0) {                /* initialise c(0) */
        c_cache[0] = (seq_t)1;
        computed   = 0;
    }

    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t sum = 0;
        seq_t bin = 1;                   /* binom(i,0) */

        for (idx_t k = 0; k < i; ++k) {
            sum += bin * c_cache[k];
            if (k + 1 < i) {
                /* bin = binom(i,k+1) = binom(i,k) * (i-k) / (k+1) */
                bin = bin * (seq_t)(i - k);
                bin = bin / (seq_t)(k + 1);
            }
        }
        c_cache[i] = sum;
    }
    computed = n;
}

/* Public entry point required by the evaluation framework. */
seq_t
a(idx_t n)
{
    if (n == 0) return (seq_t)1;  /* a(0)=1 by definition */

    compute_c_up_to(n);
    return c_cache[n] - (seq_t)n * c_cache[n - 1];
}
