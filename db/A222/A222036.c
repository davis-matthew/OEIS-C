/*
 * A222036  Number of binary arrays indicating the locations of trailing edge
 *          maxima of a random length-n 0..1 array extended with zeros and
 *          convolved with -1,2,-1.
 * https://oeis.org/A222036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-3) - a(n-5) + a(n-6) + a(n-7) + a(n-8) -
 *            2*a(n-9) - a(n-10) + a(n-11) + a(n-12); Initial values: a(1)=2,
 *            a(2)=4, a(3)=7, a(4)=12, a(5)=20, a(6)=33, a(7)=53, a(8)=85,
 *            a(9)=137, a(10)=221, a(11)=355, a(12)=570
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 45, u64 n <= 92, u128 n <= 186,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for memoisation: cache[i] holds a(i+1) */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static const seq_t base_vals[12] = {
    2, 4, 7, 12, 20, 33, 53, 85, 137, 221, 355, 570
};

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* undefined for n < 1, return 0 */

    /* Initialise cache once with known terms */
    if (cache == NULL) {
        cache_size = 12;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failed */
        for (size_t i = 0; i < 12; ++i)
            cache[i] = base_vals[i];
    }

    if (n <= 12)
        return cache[n - 1];

    /* Grow cache up to requested index */
    if ((size_t)n > cache_size) {
        size_t new_size = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failed */
        cache = tmp;
        for (size_t i = cache_size; i < new_size; ++i) {
            /* indices are 0‑based: cache[i] = a(i+1) */
            idx_t k = (idx_t)(i + 1);            /* current n */
            seq_t val = 2     * cache[k - 2]      /* 2*a(k-1) */
                    -          cache[k - 4]      /* -a(k-3) */
                    -          cache[k - 6]      /* -a(k-5) */
                    +          cache[k - 7]      /* +a(k-6) */
                    +          cache[k - 8]      /* +a(k-7) */
                    +          cache[k - 9]      /* +a(k-8) */
                    - 2     * cache[k -10]      /* -2*a(k-9) */
                    -          cache[k -11]      /* -a(k-10) */
                    +          cache[k -12]      /* +a(k-11) */
                    +          cache[k -13];     /* +a(k-12) */
            cache[i] = val;
        }
        cache_size = new_size;
    }

    return cache[n - 1];
}
