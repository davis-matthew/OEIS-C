/*
 * A207840  Number of n X 3 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 0 0 and 1 1 1 vertically.
 * https://oeis.org/A207840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence for A207840:
   a(1)=6, a(2)=36, a(3)=72, a(4)=240,
   a(5)=704, a(6)=2080, a(7)=6216, a(8)=18496.
   For n>8:
   a(n) = a(n-1) + 4*a(n-2) + 5*a(n-3) + 2*a(n-4) - a(n-5) + a(n-6). */

#include <stdlib.h>

static seq_t *cache      = NULL;    /* sequence cache */
static size_t cache_cap  = 0;       /* allocated size of cache (== capacity) */
static idx_t  cache_max  = 0;       /* highest n computed (0 means none */

/* Ensure cache can hold indices up to 'needed' (inclusive). */
static void ensure_size(idx_t needed)
{
    if (needed + 1 <= cache_cap) return;         /* already enough */

    size_t new_cap = (size_t)(needed + 1);
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* memory exhaustion */
    cache      = tmp;
    cache_cap  = new_cap;
}

/* Initialize the base values for n = 1 .. 8. */
static void init_base(void)
{
    if (cache_max != 0) return;       /* already initialized */

    ensure_size(8);

    cache[0] = 0;       /* unused; convenient */
    cache[1] = 6;
    cache[2] = 36;
    cache[3] = 72;
    cache[4] = 240;
    cache[5] = 704;
    cache[6] = 2080;
    cache[7] = 6216;
    cache[8] = 18496;

    cache_max = 8;
}

/* Compute all values up to n (inclusive). */
static void recompute(idx_t n)
{
    if (n == 0) return;                /* a(0) is undefined; simply return */

    init_base();

    if (n <= cache_max) return;        /* already computed */

    ensure_size(n);

    for (idx_t k = cache_max + 1; k <= n; ++k) {
        seq_t val = cache[k-1];
        val += cache[k-2] * (seq_t)4;
        val += cache[k-3] * (seq_t)5;
        val += cache[k-4] * (seq_t)2;
        val -= cache[k-5];
        val += cache[k-6];
        cache[k] = val;
    }

    cache_max = n;
}

/* Public function, matching the required signature. */
seq_t a(idx_t n)
{
    recompute(n);
    return cache[(size_t)n];
}
