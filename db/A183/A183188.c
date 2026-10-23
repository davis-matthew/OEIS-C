/*
 * A183188  a(n) = 3*a(n-1) + a(n-3) with a(0)=1, a(1)=2, a(2)=6.
 * https://oeis.org/A183188
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-1) + a(n-3), a(0)=1, a(1)=2, a(2)=6
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* static memoisation cache */
static seq_t *cache   = NULL;   /* buffer holding pre‑computed terms */
static size_t  cap    = 0;      /* allocated size of cache          */
static size_t  length = 0;      /* number of terms actually computed */

/* initialise cache with first three terms */
static void init_cache(void)
{
    size_t init_cap = 4;                     /* start with a small buffer */
    cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
    if (!cache) exit(EXIT_FAILURE);
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)2;
    cache[2] = (seq_t)6;
    length = 3;
    cap = init_cap;
}

/* grow the buffer so it can hold at least `needed` elements */
static void ensure_capacity(size_t needed)
{
    if (cap >= needed) return;
    size_t new_cap = cap ? cap * 2 : 4;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    cache = tmp;
    cap   = new_cap;
}

/* compute all terms up to n (inclusive) */
static void compute_up_to(idx_t n)
{
    if (!cache) init_cache();           /* cache may not yet exist */
    size_t n_ui = (size_t)n;
    if (n_ui < length) return;          /* already computed */

    ensure_capacity(n_ui + 1);           /* make room for n+1 entries */

    /* start filling from the first missing index */
    for (size_t i = length; i <= n_ui; ++i)
        cache[i] = (seq_t)3 * cache[i-1] + cache[i-3];

    length = n_ui + 1;
}

/* public entry point – returns a(n) for any n >= 0 */
seq_t a(idx_t n)
{
    compute_up_to(n);
    return cache[(size_t)n];
}
