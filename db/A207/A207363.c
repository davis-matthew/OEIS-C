/*
 * A207363  Number of n X 3 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally
 *          and 0 0 1 and 0 1 1 vertically.
 * https://oeis.org/A207363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) - 6*a(n-3) + 6*a(n-5) - 2*a(n-6) -
 *            2*a(n-7) + a(n-8); initial terms: a(1)=6, a(2)=36, a(3)=90,
 *            a(4)=225, a(5)=420, a(6)=784, a(7)=1260, a(8)=2025
 * Range:     u8 n <= 4, u16 n <= 21, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* 1‑based array of computed terms */
static idx_t   max_n = 0;        /* highest index currently stored in cache */

static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        /* Allocate enough space for indices 0..n (index 0 unused) */
        cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        /* Initialise to zero (already zero‑filled, but be explicit) */
        for (idx_t i = 0; i <= n; ++i) cache[i] = 0;
        max_n = 0;
    } else if (n > max_n) {
        /* Grow cache to accommodate up to n */
        cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        /* Zero the newly added part if any */
        for (idx_t i = max_n + 1; i <= n; ++i) cache[i] = 0;
    }

    /* Seed the known first eight terms if not already present */
    static const seq_t init_terms[8] = {6, 36, 90, 225, 420, 784, 1260, 2025};
    for (idx_t i = 1; i <= 8 && i <= n; ++i) {
        if (cache[i] == 0) cache[i] = init_terms[i - 1];
    }

    /* Compute any missing terms up to n using the recurrence */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        /* For i <= 8 we already have the values, skip computation */
        if (i <= 8) continue;

        seq_t v = 0;
        v += 2 * cache[i - 1];
        v += 2 * cache[i - 2];
        v -= 6 * cache[i - 3];
        v += 6 * cache[i - 5];
        v -= 2 * cache[i - 6];
        v -= 2 * cache[i - 7];
        v += 1 * cache[i - 8];
        cache[i] = v;
    }

    max_n = (n > max_n ? n : max_n);
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n==0, but return 0 */
    ensure_cache(n);
    return cache[n];
}
