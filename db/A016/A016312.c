/*
 * A016312  Expansion of g.f. 1/((1 - 2*x)*(1 - 7*x)*(1 - 9*x)).
 * https://oeis.org/A016312
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A016312: Expansion of g.f. 1/((1 - 2*x)*(1 - 7*x)*(1 - 9*x))
   recurrence: a(0)=1, a(1)=18, a(2)=229,
                a(n)=18*a(n-1)-95*a(n-2)+126*a(n-3)   for n>=3   */
#include <stdlib.h>

static seq_t *cache = NULL;        /* cached terms a(0)..a(k)   */
static size_t cache_cap = 0;       /* allocated capacity         */
static size_t cache_len = 0;       /* number of cached terms     */

/* Ensure the cache can hold at least 'needed' elements.              */
static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap : 4;
    while (new_cap < needed) new_cap <<= 1;
    /* C++ demands an explicit cast from void* to seq_t*          */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();               /* out‑of‑memory             */
    cache = tmp;
    cache_cap = new_cap;
}

/* Return a(n) for any n >= 0 (calls may be in any order).           */
seq_t a(idx_t n)
{
    /* The cache size and indices are size_t, but n comes as idx_t.  */
    size_t needed = (size_t)n + 1;
    ensure_capacity(needed);

    while (cache_len <= n)
    {
        if (cache_len == 0) {
            cache[0] = (seq_t)1;
        } else if (cache_len == 1) {
            cache[1] = (seq_t)18;
        } else if (cache_len == 2) {
            cache[2] = (seq_t)229;
        } else {
            /* Use the recurrence: a(n)=18*a(n-1)-95*a(n-2)+126*a(n-3)
               Careful with unsigned arithmetic: compute addition
               first, then subtraction. */
            seq_t t1 = cache[cache_len - 1];  /* a(n-1)   */
            seq_t t2 = cache[cache_len - 2];  /* a(n-2)   */
            seq_t t3 = cache[cache_len - 3];  /* a(n-3)   */

            /* All operations are done in seq_t; intermediate sizes
               are safe for built‑in fixed‑width types because the
               recurrence never overflows for values that fit.  */
            cache[cache_len] = ((seq_t)18 * t1 + (seq_t)126 * t3) - (seq_t)95 * t2;
        }
        cache_len++;
    }
    return cache[(size_t)n];
}
