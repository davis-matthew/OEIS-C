/*
 * A041422  Numerators of continued fraction convergents to sqrt(227).
 * https://oeis.org/A041422
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 13, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS sequence A041422  - Numerators of continued fraction convergents to sqrt(227)
 *
 * Recurrence:
 *   a_(-2) = 0,      a_(-1) = 1
 *   a_n   = a_n * a_(n-1) + a_(n-2)
 *   with a_0 = 15
 *   for n >= 1:  a_n = 15 if n is odd, 30 if n is even (periodic CF quotients)
 *
 * The function caches previously computed terms to support arbitrary call order.
 */
#include <stdlib.h>  /* malloc, realloc, size_t */

static seq_t *seq_cache = NULL;      /* array of computed numerators */
static size_t  cache_len  = 0;        /* number of terms currently stored */
static size_t  cache_cap  = 0;        /* allocated capacity */
static seq_t   prev2 = 1;             /* a_(-1) for recurrence */
static seq_t   prev1 = 15;            /* a_0 for recurrence */

/* Ensure the cache can hold at least 'need' elements. */
static void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t newcap = cache_cap ? cache_cap : 1;
    while (newcap < need)
        newcap *= 2;
    seq_t *tmp = (seq_t *)realloc(seq_cache, newcap * sizeof(seq_t));
    if (!tmp) abort();                      /* out of memory – terminate */
    seq_cache = tmp;
    cache_cap = newcap;
}

/* Return the numerator of the nth convergent. */
seq_t a(idx_t n)
{
    /* Initialisation: first term a(0) = 15. */
    if (cache_len == 0) {
        ensure_cache_cap(1);
        seq_cache[0] = (seq_t)15;           /* a(0) */
        cache_len   = 1;
        prev2 = (seq_t)1;                   /* a_(-1) */
        prev1 = (seq_t)15;                  /* a_0  */
    }

    /* If requested index already cached, simply return it. */
    if (n < cache_len)
        return seq_cache[(size_t)n];

    /* Generate terms up to index n. */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        /* Determine the continued‑fraction quotient a_i. */
        seq_t a_i = (i & 1) ? (seq_t)15 : (seq_t)30;  /* odd -> 15, even -> 30 */

        /* p_i = a_i * prev1 + prev2 */
        seq_t curr = a_i * prev1 + prev2;

        /* Store and advance cache. */
        ensure_cache_cap(i + 1);
        seq_cache[i] = curr;

        /* Shift for next iteration. */
        prev2 = prev1;
        prev1 = curr;
    }

    /* Update cache length */
    cache_len = (size_t)n + 1;
    return seq_cache[(size_t)n];
}
