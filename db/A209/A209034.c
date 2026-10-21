/*
 * A209034  Number of 6-bead necklaces labeled with numbers -n..n allowing
 *          reversal, with sum zero and first differences in -n..n.
 * https://oeis.org/A209034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 112, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = linear recurrence (empirical) for n >= 20
 * a(n) = 2*a(n-2) + 2*a(n-3) - 3*a(n-5) - 3*a(n-6) - 2*a(n-7)
 *         + a(n-8) + 4*a(n-9) + 4*a(n-10) + a(n-11)
 *         - 2*a(n-12) - 3*a(n-13) - 3*a(n-14) + 2*a(n-16)
 *         + 2*a(n-17) - a(n-19)
 *
 * Initial values are required for n = 1 .. 19.
 * The function is memoised; it works for any call order.
 */

#include <stdlib.h>   /* for malloc, realloc */

static const seq_t base_vals[19] = {
    4, 33, 144, 471, 1237, 2798, 5648, 10483, 18174, 29863,
    46918, 71037, 104183, 148732, 207352, 283199, 379766, 501099, 651612
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* undefined, return 0 */
    if (n <= 19) return base_vals[n - 1];    /* table lookup for the first 19 terms */

    /* Memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;          /* allocated size */
    static idx_t cache_size = 0;         /* largest computed index */

    /* First initialisation (allocate at least 19 entries) */
    if (cache_size == 0) {
        cache_cap = n > 19 ? n : 19;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        for (idx_t i = 0; i < 19; ++i) cache[i] = base_vals[i];
        cache_size = 19;
    }

    /* Grow the cache if necessary */
    if (n > cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap < n) new_cap <<= 1;   /* double until large enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;          /* allocation failure: fallback zero */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute the sequence up to n (recurrence valid for n >= 20) */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        /* Positive contributions */
        seq_t pos = 0;
        pos += 2 * cache[i - 3];
        pos += 2 * cache[i - 4];
        pos +=      cache[i - 9];
        pos += 4 * cache[i -10];
        pos += 4 * cache[i -11];
        pos +=      cache[i -12];
        pos += 2 * cache[i -17];
        pos += 2 * cache[i -18];

        /* Negative contributions */
        seq_t neg = 0;
        neg += 3 * cache[i - 6];
        neg += 3 * cache[i - 7];
        neg += 2 * cache[i - 8];
        neg += 2 * cache[i -13];
        neg += 3 * cache[i -14];
        neg += 3 * cache[i -15];
        neg +=      cache[i -20];

        cache[i - 1] = pos - neg;   /* final value */
    }
    cache_size = n;

    return cache[n - 1];
}
