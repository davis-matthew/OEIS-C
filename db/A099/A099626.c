/*
 * A099626  A transform of the Pell numbers.
 * https://oeis.org/A099626
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) + 3*a(n-2) – 2*a(n-3) – a(n-4), with a(0)=0,
 *            a(1)=1, a(2)=2, a(3)=7
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 22, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Cache the values of the sequence to allow arbitrary order of calls */
static seq_t *seq_cache = NULL;
static idx_t   cache_len  = 0;

/* Extend the cache up to index n (inclusive), computing new terms on the fly */
static void
ensure_cache(idx_t n)
{
    if (n < cache_len) {
        /* already computed */
        return;
    }

    /* Allocate or grow the cache to n+1 entries */
    seq_t *tmp = (seq_t *)realloc(seq_cache, (size_t)(n + 1) * sizeof(seq_t));
    if (!tmp) {
        /* allocation failed: abort (no I/O allowed, but aborting is safe here) */
        abort();
    }
    seq_cache = tmp;

    /* Compute new entries from the current cache_len up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0)          seq_cache[i] = (seq_t)0;
        else if (i == 1)     seq_cache[i] = (seq_t)1;
        else if (i == 2)     seq_cache[i] = (seq_t)2;
        else if (i == 3)     seq_cache[i] = (seq_t)7;
        else {
            /* recurrence: 2*a(n-1) + 3*a(n-2) – 2*a(n-3) – a(n-4) */
            seq_t term = (seq_t)(2 * seq_cache[i-1])
                       + (seq_t)(3 * seq_cache[i-2])
                       - (seq_t)(2 * seq_cache[i-3])
                       - seq_cache[i-4];
            seq_cache[i] = term;
        }
    }
    cache_len = n + 1;
}

/* Public function required by the test harness */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return seq_cache[n];
}
