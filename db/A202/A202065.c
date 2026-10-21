/*
 * A202065  The number of simple labeled graphs on 2n nodes whose connected
 *          components are even length cycles.
 * https://oeis.org/A202065
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=0, and for n≥1 a(n+1) = (4n^3‑n)·a(n‑1) +
 *            (4n^2+2n)·a(n) (OEIS A202065 recurrence)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 10, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;         /* cached values */
static idx_t  capacity = 0;         /* allocated cache size */
static idx_t  computed = 0;         /* number of values already computed */

/* Ensure the cache can hold at least 'need' elements. */
static void ensure_capacity(idx_t need)
{
    if (need <= capacity)
        return;

    idx_t new_cap = capacity > 0 ? capacity : 2;
    while (new_cap <= need)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp && new_cap > 0)
        abort();      /* allocation failed – terminate */
    cache = tmp;
    capacity = new_cap;
}

/* Compute a(n) for arbitrary‑precision or 64‑bit seq_t. */
seq_t a(idx_t n)
{
    /* Handle the two trivial base cases directly. */
    if (n == 0)
    {
        /* Ensure base case is stored for future calls. */
        if (computed == 0)
        {
            ensure_capacity(2);
            cache[0] = (seq_t)1;
            cache[1] = (seq_t)0;
            computed = 2;
        }
        return cache[0];
    }
    if (n == 1)
    {
        if (computed < 2)
        {
            ensure_capacity(2);
            cache[0] = (seq_t)1;
            cache[1] = (seq_t)0;
            computed = 2;
        }
        return cache[1];
    }

    /* Ensure cache holds values up to n. */
    if (n + 1 > capacity)
        ensure_capacity(n + 1);

    /* Initialize base cases if not yet present. */
    if (computed == 0)
    {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        computed = 2;
    }
    else if (computed == 1)
    {
        cache[1] = (seq_t)0;
        computed = 2;
    }

    /* Compute missing entries using the recurrence. */
    for (idx_t i = computed; i <= n; ++i)
    {
        /* i corresponds to n in the recurrence: a(i) = a(i+1) */
        idx_t k = i - 1;                 /* k = n in recurrence */
        seq_t k_s = (seq_t)k;
        seq_t k2 = k_s * k_s;
        seq_t k3 = k2 * k_s;
        seq_t coef1 = (seq_t)4 * k3 - k_s;          /* 4k^3 - k */
        seq_t coef2 = (seq_t)4 * k2 + (seq_t)2 * k_s;/* 4k^2 + 2k */
        cache[i] = cache[i - 2] * coef1 + cache[i - 1] * coef2;
    }

    computed = n + 1;
    return cache[n];
}
