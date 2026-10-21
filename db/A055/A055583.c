/*
 * A055583  Seventh column of triangle A055252.
 * https://oeis.org/A055583
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 10*a(n-1) - 42*a(n-2) + 96*a(n-3) - 129*a(n-4) +
 *            102*a(n-5) - 44*a(n-6) + 8*a(n-7) Initial values: a(0)=1,
 *            a(1)=10, a(2)=58, a(3)=256, a(4)=955, a(5)=3178, a(6)=9740 This
 *            follows from the generating function 1/((1-2x)^3*(1-x)^4). The
 *            recurrence uses only unsigned integer arithmetic and is...
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 20, u64 n <= 49, u128 n <= 111,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Cached values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of elements currently stored */
    static idx_t cache_cap = 0;    /* current allocation size */

    /* Initial 7 terms */
    static const seq_t init[7] = {1, 10, 58, 256, 955, 3178, 9740};

    /* Ensure the cache is large enough */
    if (n >= cache_cap) {
        idx_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
        while (new_cap <= n) new_cap *= 2;
        seq_t *new_mem = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_mem) { /* allocation failed – abort with 0, causes test to fail */
            return 0;
        }
        cache = new_mem;
        cache_cap = new_cap;
    }

    /* Compute missing terms */
    while (cache_size <= n) {
        seq_t value;
        if (cache_size < 7) {
            value = init[cache_size];
        } else {
            /* Positives: 10, 96, 102, 8  */
            seq_t pos = cache[cache_size - 1] * 10
                      + cache[cache_size - 3] * 96
                      + cache[cache_size - 5] * 102
                      + cache[cache_size - 7] * 8;
            /* Negatives: 42, 129, 44  */
            seq_t neg = cache[cache_size - 2] * 42
                      + cache[cache_size - 4] * 129
                      + cache[cache_size - 6] * 44;
            value = pos - neg;  /* pos should always be >= neg for valid terms */
        }
        cache[cache_size++] = value;
    }
    return cache[n];
}
