/*
 * A129080  Expansion of g.f. x*(x^4 - 5*x^3 + 10*x^2 - 12*x + 4)/((1-x)^2*(1
 *          - 3*x + 2*x^2 - x^3)).
 * https://oeis.org/A129080
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 27, u64 n <= 53, u128 n <= 105,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A129080
   Recurrence: a(n) = 5*a(n-1) - 9*a(n-2) + 8*a(n-3) - 4*a(n-4) + a(n-5),
   for n>=6
   Initial terms (n=1..5):
   4, 8, 14, 25, 48
   (Implementation uses a dynamic cache to answer calls in any order.) */
#include <stdlib.h>

static seq_t *cache = NULL;    /* cache[0] holds a(1) */
static idx_t cached_terms = 0; /* number of cached terms */

static const seq_t init[5] = {4, 8, 14, 25, 48};

static seq_t compute_term(idx_t i, const seq_t *c)
{
    /* recurrence applied to indices 1‑based: i>=6 */
    seq_t val = (seq_t)5 * c[i-2];          /* 5*a(i-1) */
    val -= (seq_t)9 * c[i-3];              /* -9*a(i-2) */
    val += (seq_t)8 * c[i-4];              /* +8*a(i-3) */
    val -= (seq_t)4 * c[i-5];              /* -4*a(i-4) */
    val += c[i-6];                         /* +a(i-5) */
    return val;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for n<1; harmless */

    if (n <= cached_terms)                /* already cached */
        return cache[n-1];

    /* resize cache if required */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t) * n);
        if (!cache) abort();
    } else if (cached_terms < n) {
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * n);
        if (!tmp) abort();
        cache = tmp;
    }

    /* compute missing terms sequentially */
    for (idx_t i = cached_terms + 1; i <= n; ++i) {
        if (i <= 5)
            cache[i-1] = init[i-1];
        else
            cache[i-1] = compute_term(i, cache);
    }
    cached_terms = n;

    return cache[n-1];
}
