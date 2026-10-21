/*
 * A230726  Number of n X 4 0..5 arrays x(i,j) with each element horizontally
 *          or antidiagonally next to at least one element with value
 *          (x(i,j)+1) mod 6, and upper left element zero.
 * https://oeis.org/A230726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 16*a(n-1) – 12*a(n-2) + 87*a(n-3) – 918*a(n-4) +
 *            61*a(n-5) – a(n-6) Initial values: a(1)=0, a(2)=2, a(3)=30,
 *            a(4)=396, a(5)=6212, a(6)=95412
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_terms[6] = {0, 2, 30, 396, 6212, 95412};

static seq_t *cache = NULL;          /* stored terms, 0‑based index */
static size_t cache_len = 0;         /* number of terms actually computed */
static size_t cache_cap = 0;         /* allocated capacity */

/* Extend the cache so that it holds at least 'n' terms (1‑based). */
static void ensure_cache(idx_t n)
{
    size_t need = (size_t)n;          /* n is guaranteed to be small (≤10000) */
    if (need <= cache_len)
        return;

    /* grow capacity exponentially if necessary */
    if (cache_cap < need)
    {
        size_t new_cap = cache_cap ? cache_cap : 8;
        while (new_cap < need)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();           /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Fill initial terms if they aren't already set */
    if (cache_len < 6)
    {
        size_t first_missing = cache_len;
        size_t last_needed = (need < 6) ? need : 6;
        for (size_t i = first_missing; i < last_needed; ++i)
            cache[i] = init_terms[i];
        cache_len = last_needed;
    }

    /* Compute subsequent terms using the recurrence */
    for (size_t i = cache_len; i < need; ++i)
    {
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];
        seq_t a4 = cache[i - 4];
        seq_t a5 = cache[i - 5];
        seq_t a6 = cache[i - 6];

        seq_t res = 16 * a1;
        res -= 12 * a2;
        res += 87 * a3;
        res -= 918 * a4;
        res += 61 * a5;
        res -= a6;
        cache[i] = res;
    }
    cache_len = need;
}

seq_t a(idx_t n)
{
    /* offset is 1, so first term is at n=1 */
    if (n < 1)
        return 0;
    ensure_cache(n);
    return cache[n - 1];
}
