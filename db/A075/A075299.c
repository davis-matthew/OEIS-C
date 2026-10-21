/*
 * A075299  Trajectory of 290 under the Reverse and Add! operation carried out
 *          in base 4, written in base 10.
 * https://oeis.org/A075299
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = a(n-1) + reverse_base4(a(n-1)), a(0) = 290
 *            reverse_base4(x) reverses the base‑4 digits of x
 * Range:     u8 none, u16 n <= 6, u32 n <= 20, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Reverse the base‑4 representation of x and return the resulting value. */
static seq_t reverse_base4(seq_t x)
{
    seq_t rev = 0;
    while (x > (seq_t)0)
    {
        seq_t digit = x % (seq_t)4;
        rev = rev * (seq_t)4 + digit;
        x /= (seq_t)4;
    }
    return rev;
}

/* Cache for previously computed terms. */
static seq_t *cache      = NULL;   /* array of computed terms */
static idx_t cache_size  = 0;      /* number of terms stored */
static idx_t cache_cap   = 0;      /* allocated capacity */

/* Required function. */
seq_t a(idx_t n)
{
    /* initialise cache on first call */
    if (cache == NULL)
    {
        cache_cap = 64;
        cache     = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;         /* allocation failure */
        cache[0]  = (seq_t)290;              /* a(0) */
        cache_size = 1;
    }

    /* return previously computed value if available */
    if (n < cache_size)
        return cache[(size_t)n];

    /* compute terms up to n */
    for (idx_t i = cache_size; i <= n; ++i)
    {
        seq_t last = cache[i - 1];
        seq_t rev  = reverse_base4(last);
        seq_t next = last + rev;

        /* grow cache if necessary */
        if (cache_size >= cache_cap)
        {
            cache_cap <<= 1;
            seq_t *tmp = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
            if (!tmp) return cache[(size_t)(cache_size - 1)];
            cache = tmp;
        }

        cache[cache_size++] = next;
    }

    return cache[(size_t)n];
}
