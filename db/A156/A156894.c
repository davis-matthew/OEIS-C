/*
 * A156894  a(n) = Sum_{k=0..n} binomial(n,k)*binomial(2*n+k-1,k).
 * https://oeis.org/A156894
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) * binomial(2*n + k - 1, k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Simple cache of already computed terms */
static seq_t *cache = NULL;   /* array of values */
static idx_t  len   = 0;       /* number of values stored (next free index) */
static size_t cap   = 0;       /* allocated capacity */

/* Helper to grow the cache to accommodate up to index 'n' (inclusive) */
static void grow_cache(idx_t n) {
    if ((idx_t)(cap) >= n + 1) return;
    size_t newcap = cap ? cap : 8;
    while (newcap <= (size_t)(n + 1)) newcap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    if (!tmp) { /* allocation failure – leave cache unchanged */ }
    else {
        cache = tmp;
        cap   = newcap;
    }
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Return cached value if already available */
    if (n < len) return cache[n];

    /* Initialise cache if necessary */
    if (cache == NULL) {
        cap   = 8;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;
    }

    /* Grow cache to hold all values up to 'n' */
    grow_cache(n);

    /* Compute the sequence for all indices i = len .. n */
    for (idx_t i = len; i <= n; ++i) {
        seq_t sum = 0;          /* running total of the sum */
        seq_t c   = 1;          /* binom(i, 0)   */
        seq_t d   = 1;          /* binom(2*i-1, 0) */

        for (idx_t k = 0; k <= i; ++k) {
            /* Add the current term to the sum */
            sum += c * d;

            /* Update binomial values for next k, if any */
            if (k == i) break;

            /* For binom(i, k+1) = binom(i, k) * (i - k) / (k + 1) */
            seq_t numer_c = c * (seq_t)(i - k);
            seq_t denom   = (seq_t)(k + 1);
            c = numer_c / denom;

            /* For binom(2*i + k, k+1) = binom(2*i + k - 1, k) *
               (2*i + k) / (k + 1) */
            seq_t numer_d = d * (seq_t)(2 * i + k);
            d = numer_d / denom;
        }

        cache[i] = sum;
    }

    len = n + 1;               /* all values up to n are now cached */
    return cache[n];
}
