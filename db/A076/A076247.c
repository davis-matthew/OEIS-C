/*
 * A076247  Trajectory of 1059774 under the Reverse and Add! operation carried
 *          out in base 4, written in base 10.
 * https://oeis.org/A076247
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(0) = 1059774, a(n+1) = a(n) + reverse_base4(a(n))
 * Range:     u8 none, u16 none, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* reverse_base4(x) reverses the base‑4 digits of x and returns the new value. */

#include <stdlib.h>

static seq_t *cache = NULL;   /* stored terms of the sequence */
static size_t cache_len = 0;   /* number of terms stored */
static size_t cache_cap = 0;   /* allocated capacity */

/* Reverse the base‑4 digits of x. */
static inline seq_t reverse_base4(seq_t x)
{
    seq_t rev = 0;
    seq_t t = x;
    const seq_t FOUR = (seq_t)4;
    while (t != 0)
    {
        seq_t d = t % FOUR;
        rev = rev * FOUR + d;
        t   = t / FOUR;
    }
    return rev;
}

/* Compute terms up to index n (inclusive). */
static inline void ensure_cache(size_t n)
{
    if (n < cache_len)
        return;                      /* already computed up to n */

    /* Increase capacity if needed. */
    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap;
    while (new_cap <= n)
        new_cap <<= 1;               /* double until enough space */

    if (new_cap > cache_cap)
    {
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();                 /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* First term initialization. */
    if (cache_len == 0)
    {
        cache[0] = (seq_t)1059774;
        cache_len = 1;
    }

    /* Compute subsequent terms. */
    for (size_t i = cache_len; i <= n; ++i)
    {
        seq_t next = cache[i - 1] + reverse_base4(cache[i - 1]);
        cache[i] = next;
    }
    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache((size_t)n);
    return cache[n];
}
