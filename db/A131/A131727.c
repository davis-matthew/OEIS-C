/*
 * A131727  Pell numbers A000129 without last digit.
 * https://oeis.org/A131727
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = floor( Pell(n) / 10 ) Pell(n): P(0)=0, P(1)=1,
 *            P(n)=2*P(n-1)+P(n-2) (n≥2) a(4) is first non‑nonsense value in
 *            the OEIS listing
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 26, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for the sequence values a(n) for n ≥ 4.
   cache[i] == a(i+4)  (i = 0,1,2,…,last_index-4) */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* current capacity of cache */
static size_t cache_len = 0;   /* number of stored elements (last_index-3) */

/* Tracking the last computed Pell numbers:
   P_last1 = P(last_index), P_last2 = P(last_index-1) */
static idx_t last_index = 3;          /* P(3) known, nothing stored yet for n≥4 */
static seq_t P_last1 = (seq_t)5;      /* P(3) */
static seq_t P_last2 = (seq_t)2;      /* P(2) */

/* Ensure the cache has room for at least 'need' elements. */
static void ensure_capacity(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the nth term of A131727, 0 for n < 4. */
seq_t a(idx_t n)
{
    if (n < 4)          /* behaviour below offset is unspecified */
        return 0;

    if (n > last_index)
    {
        /* We need values from last_index+1 up to n. */
        size_t need = (n - 3); /* cache needs to hold indices 4..n inclusive */
        ensure_capacity(need);

        for (idx_t i = last_index + 1; i <= n; ++i)
        {
            /* Pell recurrence: P(i) = 2*P(i-1) + P(i-2) */
            seq_t P_new = (P_last1 << 1) + P_last2;
            seq_t val    = P_new / 10;          /* floor(P(i)/10) */

            cache[i - 4] = val;                 /* store a(i) */

            /* Advance the P register for next iteration. */
            P_last2 = P_last1;
            P_last1 = P_new;
        }
        last_index = n;
        cache_len = last_index - 3;
    }

    return cache[n - 4];
}
