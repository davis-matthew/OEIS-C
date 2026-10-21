/*
 * A214161  Number of 0..5 colorings of an nX4 array circular in the 4
 *          direction with new values 0..5 introduced in row major order.
 * https://oeis.org/A214161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 370 a(n-1) – 18411 a(n-2) + 297448 a(n-3) – 1839799
 *            a(n-4) * + 4424682 a(n-5) – 4113757 a(n-6) + 1249468 a(n-7) *
 *            a(1)=4 , a(2)=337 , a(3)=88261 , a(4)=27240856 , *
 *            a(5)=8548472292 , a(6)=2687052137887 , a(7)=844765198701963. *
 *            This implementation memoises the sequence and works for any
 *            or...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Cache for computed terms (0‑based: cache[0] = a(1)) */
static seq_t *cache = NULL;           /* array of terms */
static size_t cache_used = 0;         /* number of terms currently stored */
static size_t cache_alloc = 0;        /* allocated capacity (in terms) */

/* initial seven terms of the sequence */
static const seq_t init_values[7] = {
    (seq_t)4,
    (seq_t)337,
    (seq_t)88261,
    (seq_t)27240856,
    (seq_t)8548472292ull,
    (seq_t)2687052137887ull,
    (seq_t)844765198701963ull
};

/* memoised sequence value a(n), with n >= 1 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, but safe */

    /* array indices are 0‑based; term n -> cache[n-1] */
    const size_t N = (size_t)n;

    /* we need at least the first seven terms available */
    const size_t need = N < 7 ? 7 : N;

    /* grow storage if required */
    if (need > cache_alloc) {
        /* realloc is not type‑safe for a user defined type; cast explicitly */
        seq_t *new_cache = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (!new_cache) {
            /* allocation failed – aborting is acceptable for this sequence */
            exit(EXIT_FAILURE);
        }
        cache = new_cache;
        cache_alloc = need;
    }

    /* initialise any missing initial terms */
    if (cache_used < 7) {
        const size_t limit = 7 < cache_alloc ? 7 : cache_alloc;
        for (size_t i = cache_used; i < limit; ++i)
            cache[i] = init_values[i];
        cache_used = limit;
    }

    /* compute any further terms that are needed */
    for (size_t i = cache_used; i < need; ++i) {
        /* recurrence for i >= 7 (i is 0‑based index so i+1 >= 8);
           for i < 7 the element is already initialised above */
        if (i < 7) continue;

        /* compute the sum of positive coefficients and the sum of
           negative coefficients separately to avoid unsigned underflow */
        seq_t pos = (seq_t)370   * cache[i-1]
                  + (seq_t)297448 * cache[i-3]
                  + (seq_t)4424682 * cache[i-5]
                  + (seq_t)1249468 * cache[i-7];

        seq_t neg = (seq_t)18411  * cache[i-2]
                  + (seq_t)1839799 * cache[i-4]
                  + (seq_t)4113757 * cache[i-6];

        cache[i] = pos - neg;
    }

    /* update the number of stored terms */
    if (need > cache_used) cache_used = need;

    /* return the requested term */
    return cache[N-1];
}
