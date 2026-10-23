/*
 * A102129  Expansion of (1 - x)*(1 + 2*x) / ((1 + x)*(1 - 4*x - x^2)).
 * https://oeis.org/A102129
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A102129 – Expansion of (1 - x)*(1 + 2*x) / ((1 + x)*(1 - 4*x - x^2))
 *
 * Recurrence derived from the generating function:
 *   a(0) = 1
 *   a(1) = 4
 *   a(2) = 15
 *   a(n) = 3*a(n-1) + 5*a(n-2) + a(n-3)      for n >= 3
 *
 * The function memoises computed terms so that
 *   a(n)   = a(n-1) + a(n-2) + a(n-3)   is evaluated only once per
 *          needed term.  The routine works for any seq_t/idx_t
 *          types supplied by the build environment.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* array of previously computed terms   */
static idx_t  cached_len = 0;    /* number of allocated entries          */

/* compute terms up to n (inclusive) using the recurrence above */
static void ensure_cache(idx_t n)
{
    /* initialise to the first three known terms when required */
    if (cached_len == 0) {
        cache = (seq_t *)malloc(3 * sizeof(seq_t));
        if (!cache) { cached_len = 0; return; }
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)15;
        cached_len = 3;
    }

    /* return if already in cache */
    if (n < cached_len) return;

    /* grow the cache to hold up to index n */
    idx_t new_len = n + 1;                         /* indices 0 … n */
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) { cached_len = 0; return; }        /* allocation failure – leave incomplete */
    cache = tmp;

    /* compute any missing terms using the recurrence */
    for (idx_t i = cached_len; i <= n; ++i) {
        /* a(i) = 3*a(i-1) + 5*a(i-2) + a(i-3)  (valid for i >= 3) */
        seq_t t1 = cache[i-1];
        seq_t t2 = cache[i-2];
        seq_t t3 = cache[i-3];
        cache[i] = ((seq_t)3 * t1) + ((seq_t)5 * t2) + t3;
    }
    cached_len = new_len;
}

/* external linkage required by the evaluator */
seq_t a(idx_t n)
{
    ensure_cache(n);
    if (n >= cached_len) return (seq_t)0;   /* safety fall‑back if allocation failed */
    return cache[n];
}
