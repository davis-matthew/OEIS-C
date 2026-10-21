/*
 * A086022  a(n) = Sum_{i=1..n} C(i+2,3)^4.
 * https://oeis.org/A086022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{i=1..n} C(i+2,3)^4 (C(k,3) = k*(k-1)*(k-2)/6). The
 *            sequence is computed by accumulating the fourth power of the
 *            binomial coefficient for each i. A cache of prefix sums is kept
 *            so that subsequent calls are O(1) after the first computation up
 *            to the largest n seen. The cache grows autom...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 10, u64 n <= 62, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static seq_t *cache = NULL;          /* 1‑based, cache[0] unused          */
static idx_t  computed_up_to = 0;    /* largest i for which cache[i] is valid */

/* Make sure cache can hold indices up to n (inclusive).  The cache
   size is always a power of two (except the first allocation). */
static void ensure_cache_size(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (cache && (size_t)computed_up_to + 1 >= needed)
        return;

    size_t new_len = (cache == NULL) ? 16 : (size_t)computed_up_to + 1;
    while (new_len < needed)
        new_len <<= 1;

    seq_t *new_cache;
#ifdef __cplusplus
    new_cache = new seq_t[new_len];
#else
    new_cache = (seq_t*)malloc(sizeof(seq_t) * new_len);
    if (!new_cache) abort();
#endif
    /* copy existing values */
    if (cache) {
        for (size_t i = 0; i <= (size_t)computed_up_to; ++i)
            new_cache[i] = cache[i];
    }
    /* free old cache */
    if (cache) {
#ifdef __cplusplus
        delete[] cache;
#else
        free(cache);
#endif
    }
    cache = new_cache;
    /* No need to initialise the rest; values will be written before use */
}

/* Compute a(n).  n >= 1.  Wrong indices return 0. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* grow cache if necessary */
    ensure_cache_size(n);

    /* Compute missing prefix sums up to n. */
    for (idx_t i = computed_up_to + 1; i <= n; ++i) {
        /* binom = C(i+2,3) = (i+2)*(i+1)*i / 6 */
        seq_t i_seq   = (seq_t)i;
        seq_t binom   = ((i_seq + (seq_t)2) * (i_seq + (seq_t)1) * i_seq) / (seq_t)6;
        seq_t term    = binom * binom * binom * binom;  /* binom^4 */

        if (i == 1)
            cache[i] = term;           /* a(1) */
        else
            cache[i] = cache[i - 1] + term; /* a(i) = a(i-1)+term */
    }
    computed_up_to = n;
    return cache[n];
}
