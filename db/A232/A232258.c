/*
 * A232258  Number of (1+1)X(n+1) 0..2 arrays with every element both >= and
 *          <= some horizontal or antidiagonal neighbor.
 * https://oeis.org/A232258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) -3*a(n-2) +19*a(n-3) -16*a(n-4) +13*a(n-5) *
 *            -45*a(n-6) -10*a(n-7) -26*a(n-8) * -11*a(n-9) +26*a(n-10)
 *            +4*a(n-11) * -4*a(n-12) * Initial values (non‑recursive, for 1 ≤
 *            n ≤ 13): * a(1)=9, a(2)=37, a(3)=215, a(4)=1287, a(5)=7673, *
 *            a(6)=45709, a(7)=272417, a(8)=1623601, a(9)=9676571...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Initial terms (index 0 unused) */
static const seq_t init_vals[14] = {
    0ULL,                      /* a(0) unused */
    9ULL, 37ULL, 215ULL, 1287ULL, 7673ULL,
    45709ULL, 272417ULL, 1623601ULL, 9676571ULL,
    57671897ULL, 343721665ULL, 2048564473ULL,
    12209345763ULL
};

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* cache[0] is unused; cache[i] holds a(i) for i ≥ 1 */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;          /* number of allocated elements */

    /* target size needed to index n (1‑based) */
    size_t target = (size_t)n + 1;

    /* Allocate or grow cache as necessary */
    size_t old_len = cache_len;
    if (cache_len < target) {
        seq_t *tmp = (seq_t *)realloc(cache, target * sizeof(seq_t));
        if (!tmp) return 0;               /* allocation failure */
        cache = tmp;
        /* Initialize newly allocated slots to 0 */
        for (size_t i = old_len; i < target; ++i) cache[i] = 0;
        cache_len = target;
    }

    /* Ensure initial values (up to 13) are present */
    for (size_t i = 1; i <= 13 && i < cache_len; ++i)
        if (cache[i] == 0) cache[i] = init_vals[i];

    /* Compute any terms that have not yet been calculated */
    size_t start = old_len > 14 ? old_len : 14;
    for (size_t idx = start; idx <= n; ++idx) {
        seq_t pos = cache[idx - 1] * 6
                  + cache[idx - 3] * 19
                  + cache[idx - 5] * 13
                  + cache[idx -10] * 26
                  + cache[idx -11] * 4;

        seq_t neg = cache[idx - 2] * 3
                  + cache[idx - 4] * 16
                  + cache[idx - 6] * 45
                  + cache[idx - 7] * 10
                  + cache[idx - 8] * 26
                  + cache[idx - 9] * 11
                  + cache[idx -12] * 4;

        cache[idx] = pos - neg;
    }

    return cache[n];
}
