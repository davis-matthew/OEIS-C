/*
 * A211254  Number of (n+1) X (n+1) -6..6 symmetric matrices with every 2 X 2
 *          subblock having sum zero and three distinct values.
 * https://oeis.org/A211254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 28, u64 n <= 60, u128 n <= 124,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 5*a(n-1) - 36*a(n-3) + 41*a(n-4) + 82*a(n-5)
   - 151*a(n-6) - 54*a(n-7) + 204*a(n-8) - 28*a(n-9) - 110*a(n-10)
   + 36*a(n-11) + 20*a(n-12) - 8*a(n-13),  n ≥ 14,
   with a(1)=64, a(2)=130, … ,a(13)=120752. */
#include <stdlib.h>

/* Initial values a(1) .. a(13); a(0) is unused. */
static const seq_t init_vals[14] = {0U,
    64U, 130U, 246U, 456U, 840U, 1538U, 2830U, 5214U,
    9672U, 18020U, 33790U, 63702U, 120752U};

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cached = 0;   /* highest index currently filled in cache */

    /* Initialise cache with the first 13 terms if not already done. */
    if (cache == NULL) {
        cached = 13;
        cache = (seq_t *)malloc((cached + 1) * sizeof(seq_t));
        if (cache == NULL) abort();
        for (idx_t i = 1; i <= cached; ++i)
            cache[i] = init_vals[i];
        cache[0] = init_vals[0];   /* unused but harmless */
    }

    /* Return already computed term if within cache. */
    if (n <= cached) return cache[n];

    /* Grow cache to include term n, computing new terms with the recurrence. */
    seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (tmp == NULL) abort();
    cache = tmp;

    for (idx_t i = cached + 1; i <= n; ++i) {
        seq_t pos =  5 * cache[i-1]
                  + 41 * cache[i-4]
                  + 82 * cache[i-5]
                  + 204 * cache[i-8]
                  + 36 * cache[i-11]
                  + 20 * cache[i-12];
        seq_t neg = 36 * cache[i-3]
                  + 151 * cache[i-6]
                  + 54 * cache[i-7]
                  + 28 * cache[i-9]
                  + 110 * cache[i-10]
                  + 8  * cache[i-13];
        cache[i] = pos - neg;
    }

    cached = n;
    return cache[n];
}
