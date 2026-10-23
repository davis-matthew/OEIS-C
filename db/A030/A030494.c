/*
 * A030494  If n is even, 2(n/2 + 1)! - 1; if n is odd, ((n + 1)/2 + 1)! - 1.
 * https://oeis.org/A030494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n even) ? 2 * ((n/2)+1)! - 1 : ((n+1)/2+1)! - 1 (index
 *            starts at 1)
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 22, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache of factorials: fact_cache[i] = i! for i <= fact_cache_size */
static seq_t *fact_cache = NULL;
static idx_t fact_cache_size = 0;

/* Initialise the cache with 0! = 1 and 1! = 1 if not yet allocated. */
static void init_fact_cache(void)
{
    if (fact_cache != NULL)
        return;

    fact_cache = (seq_t *)malloc(2 * sizeof(seq_t));
    if (fact_cache == NULL)
        return;                 /* allocation failed – values will be zero */

    fact_cache[0] = (seq_t)1;
    fact_cache[1] = (seq_t)1;
    fact_cache_size = 1;          /* highest index stored */
}

/* Extend the cache so that it contains factorials up to 'needed'. */
static void ensure_fact_cache(idx_t needed)
{
    if (needed <= fact_cache_size)
        return;                  /* already computed */

    idx_t new_size = needed + 1;  /* number of elements after growth */
    seq_t *tmp = (seq_t *)realloc(fact_cache, new_size * sizeof(seq_t));
    if (tmp == NULL)
        return;                  /* allocation failed – leave cache unchanged */

    fact_cache = tmp;

    /* Compute factorials that were missing. */
    seq_t val = fact_cache[fact_cache_size];
    for (idx_t i = fact_cache_size + 1; i <= needed; ++i) {
        val = val * (seq_t)i;    /* val now equals i! */
        fact_cache[i] = val;
    }

    fact_cache_size = needed;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* n ≥ 1: compute the appropriate argument m for the factorial */
    idx_t m = (n & 1U) ? ((n + 1) / 2 + 1) : (n / 2 + 1);

    init_fact_cache();
    ensure_fact_cache(m);

    seq_t fact = fact_cache[m];

    if (n & 1U)          /* n is odd */
        return fact - (seq_t)1;
    else                /* n is even */
        return fact * (seq_t)2 - (seq_t)1;
}
