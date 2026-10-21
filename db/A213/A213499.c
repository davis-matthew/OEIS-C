/*
 * A213499  Number of (w,x,y) with all terms in {0,...,n} and w != min(|w-x|,
 *          |x-y|).
 * https://oeis.org/A213499
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 2*a(n-2) - a(n-3) - 2*a(n-4) - a(n-5) + 2*a(n-6)
 *            + a(n-7) - a(n-8) initial values: a(0)=0, a(1)=4, a(2)=17,
 *            a(3)=47, a(4)=96, a(5)=176, a(6)=286, a(7)=439
 * Range:     u8 n <= 5, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* static cache, its current size and last computed index */
static seq_t *cache = NULL;
static size_t cap = 0;
static idx_t last = 0;
static int initialized = 0;

/* enlarge cache so that it can hold indices 0..need (inclusive) */
static void enlarge_cache(idx_t need)
{
    if ((size_t)need + 1 <= cap) return;

    size_t newcap = cap ? cap * 2 : 64;
    while (newcap < (size_t)need + 1) newcap <<= 1;

    seq_t *newp = (seq_t *)malloc(newcap * sizeof(seq_t));
    if (!newp) abort();   /* allocation failure */

    /* copy old data */
    for (size_t i = 0; i < cap; ++i) newp[i] = cache[i];
    free(cache);
    cache = newp;
    cap = newcap;
}

/* implementation */
seq_t a(idx_t n)
{
    /* lazy static initialization */
    if (!initialized) {
        /* store the known first eight terms (no compile‑time truncation) */
        static const int init_vals[8] = { 0, 4, 17, 47, 96, 176, 286, 439 };

        enlarge_cache(7);                      /* capacity for indices 0..7 */
        for (idx_t i = 0; i <= 7; ++i) cache[i] = (seq_t)init_vals[(int)i];
        last = 7;
        initialized = 1;
    }

    if (n <= last) return cache[n];

    enlarge_cache(n);
    for (idx_t k = last + 1; k <= n; ++k) {
        if (k <= 7) {                 /* safety: already set in init */
            cache[k] = (seq_t)k;      /* placeholder; will be overwritten below if needed */
            continue;
        }
        /* recurrence for k >= 8 */
        cache[k] = cache[k-1]
                 + 2 * cache[k-2]
                 - cache[k-3]
                 - 2 * cache[k-4]
                 - cache[k-5]
                 + 2 * cache[k-6]
                 + cache[k-7]
                 - cache[k-8];
    }
    last = n;
    return cache[n];
}
