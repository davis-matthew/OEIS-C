/*
 * A162009  Fourth right hand column of the EG1 triangle A162005.
 * https://oeis.org/A162009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 130*a(n-1) - 6501*a(n-2) + 163160*a(n-3) - 2236466*a(n-4)
 *            + 17123340*a(n-5) - 71497186*a(n-6) + 154127320*a(n-7) -
 *            174334221*a(n-8) + 98986050*a(n-9) - 22325625*a(n-10) Initial
 *            values: a(1)=272, a(2)=52736, a(3)=4766048, a(4)=319830400,
 *            a(5)=18598875760, a(6)=1002968825344, a(7)=5188263875...
 * Range:     u8 none, u16 n <= 2, u32 n <= 4, u64 n <= 10, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;       /* 1‑indexed cache, cache[0] unused */
static idx_t  cache_up_to = 0;    /* highest n computed so far */
static size_t cache_cap   = 0;    /* allocated capacity (in entries) */

/* Initialise cache with the first 10 terms. */
static void init_cache(void)
{
    if (cache) return;               /* already initialised */

    /* allocate a little more space for future growth */
    cache_cap = 32;
    cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    if (!cache) return;              /* silently abort on OOM – caller never expects this */

    /* first 10 terms, cast from integer literals */
    cache[0] = (seq_t)0;                   /* unused – keeps indices 1‑based */
    cache[1] = (seq_t)272;
    cache[2] = (seq_t)52736;
    cache[3] = (seq_t)4766048;
    cache[4] = (seq_t)319830400;
    cache[5] = (seq_t)18598875760ULL;
    cache[6] = (seq_t)1002968825344ULL;
    cache[7] = (seq_t)51882638754240ULL;
    cache[8] = (seq_t)2621627565515520ULL;
    cache[9] = (seq_t)130715075544000720ULL;
    cache[10] = (seq_t)6468157990602644480ULL;

    cache_up_to = 10;
}

/* Grow the cache array if n+1 exceeds its capacity. */
static void ensure_capacity(idx_t n)
{
    if (!cache) init_cache();
    if ((size_t)(n + 1) <= cache_cap) return;

    size_t new_cap = cache_cap;
    while ((size_t)(n + 1) > new_cap) new_cap *= 2;
    cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Return the n‑th term of A162009 (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* offset safety – unused */

    ensure_capacity(n);

    if (n <= cache_up_to) return cache[n];

    /* compute terms from cache_up_to+1 up to n using the recurrence */
    for (idx_t i = cache_up_to + 1; i <= n; ++i)
    {
        seq_t term = cache[i-1] * (seq_t)130
                   - cache[i-2] * (seq_t)6501
                   + cache[i-3] * (seq_t)163160
                   - cache[i-4] * (seq_t)2236466
                   + cache[i-5] * (seq_t)17123340
                   - cache[i-6] * (seq_t)71497186
                   + cache[i-7] * (seq_t)154127320
                   - cache[i-8] * (seq_t)174334221
                   + cache[i-9] * (seq_t)98986050
                   - cache[i-10] * (seq_t)22325625;
        cache[i] = term;
    }

    cache_up_to = n;
    return cache[n];
}
