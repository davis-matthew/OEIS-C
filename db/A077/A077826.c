/*
 * A077826  Expansion of (1-x)^(-1)/(1-2*x-3*x^2-2*x^3).
 * https://oeis.org/A077826
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + a(n-2) - a(n-3) - 2*a(n-4) Initial: a(0)=1,
 *            a(1)=3, a(2)=10, a(3)=32
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq_cache = NULL;     /* memoised values */
static size_t seq_cache_cap = 0;    /* allocated capacity */
static idx_t seq_cache_max = (idx_t)-1; /* highest index stored */

/* initialise cache with the first four terms */
static void seq_init(void)
{
    if (seq_cache != NULL)
        return;
    seq_cache_cap = 4;
    seq_cache = (seq_t *)malloc(seq_cache_cap * sizeof(seq_t));
    if (!seq_cache)
        abort();
    seq_cache[0] = (seq_t)1;
    seq_cache[1] = (seq_t)3;
    seq_cache[2] = (seq_t)10;
    seq_cache[3] = (seq_t)32;
    seq_cache_max = 3;
}

/* grow the cache so that it can hold indices up to n */
static void seq_ensure_capacity(idx_t n)
{
    if ((size_t)n < seq_cache_cap)
        return;
    size_t new_cap = seq_cache_cap ? seq_cache_cap * 2 : 4;
    while (new_cap <= (size_t)n)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    seq_cache = tmp;
    seq_cache_cap = new_cap;
}

/* public entry point */
seq_t a(idx_t n)
{
    seq_init();
    if (n <= seq_cache_max)
        return seq_cache[(size_t)n];
    seq_ensure_capacity(n);
    for (size_t i = (size_t)(seq_cache_max + 1); i <= (size_t)n; ++i)
    {
        seq_t val = 3 * seq_cache[i - 1]
                    + seq_cache[i - 2]
                    - seq_cache[i - 3]
                    - 2 * seq_cache[i - 4];
        seq_cache[i] = val;
    }
    seq_cache_max = n;
    return seq_cache[(size_t)n];
}
