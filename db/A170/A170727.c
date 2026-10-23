/*
 * A170727  Number of reduced words of length n in Coxeter group on 46
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^50 = I.
 * https://oeis.org/A170727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence * a(0) = 1 * a(n) = 44
 *            a(n-1)+44 a(n-2)+…+44 a(n-49)-990 a(n-50) + p(n) * where
 *            p(0)=p(50)=1, p(1…49)=2, and p(n)=0 for n>50. * This follows
 *            from the rational generating function of the * Coxeter group
 *            described in OEIS A170727. The recurrence is * of order 50...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* The build supplies the integer types seq_t and idx_t. */
static seq_t *cache = NULL;     /* Memoised values a[0..] */
static idx_t cache_size = 0;    /* Number of cached entries */

/* Ensure that cache contains values a[0..n].  Compute missing entries
 * sequentially using the recurrence. */
static void ensure_cache(idx_t n)
{
    if (n < cache_size) return;                      /* Already computed */

    /* Grow the cache */
    idx_t new_size = n + 1;
    if (cache == NULL)
        cache = (seq_t*)malloc(new_size * sizeof(seq_t));
    else
        cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));

    /* Compute new terms */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t val;
        if (i == 0)            val = (seq_t)1;          /* p(0) */
        else if (i == 50)       val = (seq_t)1;          /* p(50) */
        else if (i < 50)        val = (seq_t)2;          /* p(1..49) */
        else                    val = (seq_t)0;          /* p(n>=51) */

        /* Add contributions from previous terms */
        for (unsigned j = 1; j <= 49; ++j) {
            if (i >= j) val += (seq_t)44 * cache[i - j];
        }
        if (i >= 50) val -= (seq_t)990 * cache[i - 50];

        cache[i] = val;
    }

    cache_size = new_size;
}

/* Public function required by the testing framework. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
