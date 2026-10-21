/*
 * A209487  Number of 6-bead necklaces labeled with numbers -n..n allowing
 *          reversal, with sum zero and avoiding the patterns z z+1 z+2 and z
 *          z-1 z-2.
 * https://oeis.org/A209487
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) -10*a(n-2) +11*a(n-3) -10*a(n-4) +11*a(n-5)
 *            -10*a(n-6) +5*a(n-7) - a(n-8) for n ≥ 10 Initial values: a(1)=
 *            11, a(2)=136, a(3)=731, a(4)=2606, a(5)=7179, a(6)=16660,
 *            a(7)=34233, a(8)=64220, a(9)=112263
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 77, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache    = NULL;   /* cache[1] … cache[maxcalc] */
static size_t  cap     = 0;      /* allocated size, >= required +1 */
static idx_t   maxcalc = 0;      /* largest index with computed value */

/* Ensure capacity for index n (1‑based). Allocate at least n+1 elements. */
static void ensure_capacity(idx_t n)
{
    if ((size_t)n + 1 <= cap) return;

    size_t old_cap = cap;
    size_t new_cap = cap ? cap * 2 : 16;
    while (new_cap <= (size_t)n + 1) new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();            /* out of memory */
    cache = tmp;
    cap   = new_cap;
    /* zero any newly allocated slots */
    for (size_t i = old_cap; i < cap; ++i) cache[i] = 0;
}

/* Initialise the first nine terms if they are not already present. */
static void init_first_terms(void)
{
    if (maxcalc >= 9) return;
    ensure_capacity(9);
    cache[1] = 11;
    cache[2] = 136;
    cache[3] = 731;
    cache[4] = 2606;
    cache[5] = 7179;
    cache[6] = 16660;
    cache[7] = 34233;
    cache[8] = 64220;
    cache[9] = 112263;
    maxcalc  = 9;
}

/* Compute terms up to index n using the empirical recurrence. */
static void extend_cache(idx_t n)
{
    if (n <= maxcalc) return;
    for (idx_t i = maxcalc + 1; i <= n; ++i)
    {
        /* recurrence applies for i ≥ 10; for i = 10, all required terms are set */
        seq_t val = cache[i-1] * 5
                  - cache[i-2] * 10
                  + cache[i-3] * 11
                  - cache[i-4] * 10
                  + cache[i-5] * 11
                  - cache[i-6] * 10
                  + cache[i-7] * 5
                  - cache[i-8];
        cache[i] = val;
    }
    maxcalc = n;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n<1, return 0 */
    ensure_capacity(n);
    init_first_terms();
    extend_cache(n);
    return cache[n];
}
