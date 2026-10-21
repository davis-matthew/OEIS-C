/*
 * A071047  Number of 1's in n-th row of triangle in A071031, cellular
 *          automaton "rule 62".
 * https://oeis.org/A071047
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+7) = a(n+4) + a(n+3) – a(n) ( n ≥ 0 ) Initial values:
 *            a(0)=1, a(1)=3, a(2)=3, a(3)=6, a(4)=5, a(5)=8, a(6)=9 The
 *            function below computes a(n) for any n ≥ 0, using memoisation
 *            and the linear recurrence to stay O(n)
 * Range:     u8 n <= 203, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for memoised terms */
static seq_t *cache   = NULL;   /* array of terms */
static idx_t cache_len = 0;     /* first cache_len indices are valid */

/* Ensure that cache contains a(i) for i = 0 .. n inclusive.  */
static void ensure_cache(idx_t n)
{
    if (n < cache_len)                /* already computed */
        return;

    /* At least the first 7 terms are needed for the recurrence.          */
    idx_t required = n + 1;
    if (required < 7)
        required = 7;

    seq_t *tmp = (seq_t *)realloc(cache, required * sizeof(seq_t));
    if (!tmp)
        abort();
    cache = tmp;

    /* Initialise the first seven terms once.  */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)6;
        cache[4] = (seq_t)5;
        cache[5] = (seq_t)8;
        cache[6] = (seq_t)9;
        cache_len = 7;
    }

    /* Compute the remaining terms up to n using the recurrence.  */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* i‑4, i‑3, i‑7 are valid because i ≥ 7 at this point.      */
        cache[i] = cache[i - 4] + cache[i - 3] - cache[i - 7];
    }

    cache_len = required;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
