/*
 * A121649  G.f.: A(x) = 1/(1 - x*B(x^2)), where B(x) = Sum_{n>=0} a(n)^2*x^n
 *          is the g.f. of A121648.
 * https://oeis.org/A121649
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n>=1, a(n) = Σ_{m=0..⌊(n-1)/2⌋} a(n-2m-1) * a(m)^2
 *            (OEIS A121649)
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Static cache for computed terms. */
static seq_t *seq_cache = NULL;      /* cache array */
static size_t seq_cache_size = 0;    /* current allocated size of the cache */
static idx_t  seq_computed_up = 0;   /* highest index already computed (≥0) */
static bool   seq_initialized = false;   /* flag to initialise a(0) once */

/* Ensure the cache can hold values up to index `n`. */
static void seq_ensure_cache(idx_t n)
{
    if ((size_t)n + 1 <= seq_cache_size)
        return;  /* already large enough */

    size_t new_size = (size_t)n + 1;
    seq_t *tmp = NULL;

    if (seq_cache == NULL)
        tmp = (seq_t *)malloc(new_size * sizeof(seq_t));
    else
        tmp = (seq_t *)realloc(seq_cache, new_size * sizeof(seq_t));

    if (tmp == NULL)
        abort();  /* allocation failure */

    seq_cache = tmp;
    seq_cache_size = new_size;
}

/* Return the nth term of OEIS A121649. */
seq_t a(idx_t n)
{
    /* Initialise cache on first call. */
    if (!seq_initialized) {
        seq_ensure_cache(0);
        seq_cache[0] = (seq_t)1;            /* a(0) = 1 */
        seq_computed_up = 0;
        seq_initialized = true;
    }

    seq_ensure_cache(n);

    /* If already computed, return the value. */
    if (n <= seq_computed_up)
        return seq_cache[n];

    /* Compute missing terms up to `n`. */
    for (idx_t k = seq_computed_up + 1; k <= n; ++k) {
        seq_t acc = 0;
        idx_t m_max = (k - 1) / 2;
        for (idx_t m = 0; m <= m_max; ++m) {
            seq_t square = seq_cache[m] * seq_cache[m];
            acc += seq_cache[k - 2 * m - 1] * square;
        }
        seq_cache[k] = acc;
    }

    seq_computed_up = n;
    return seq_cache[n];
}
