/*
 * A203287  Number of arrays of 2n nondecreasing integers in -4..4 with sum
 *          zero and equal numbers greater than zero and less than zero.
 * https://oeis.org/A203287
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) -4*a(n-2) -3*a(n-3) +6*a(n-4) -6*a(n-7)
 *            +3*a(n-8) +4*a(n-9) -4*a(n-10) +a(n-11); Initial values:
 *            a(1)=5,a(2)=21,a(3)=69,a(4)=188,a(5)=444,a(6)=944,a(7)=1844,a(8)=3369,
 *            a(9)=5825,a(10)=9621,a(11)=15285
 * Range:     u8 n <= 4, u16 n <= 14, u32 n <= 106, u64 n <= 189,
 *            u128 n <= 189, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[12] = {0, 5, 21, 69, 188, 444, 944, 1844,
                                    3369, 5825, 9621, 15285};

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (n <= 11) return init_vals[n];

    static seq_t *cache = NULL;
    static size_t cache_size = 0;      /* allocated size */
    static size_t max_known = 11;      /* highest index computed */

    /* Allocate cache on first use */
    if (cache == NULL) {
        cache_size = 12;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) return 0;    /* allocation failure */
        for (size_t i = 0; i <= 11; ++i) cache[i] = init_vals[i];
    }

    /* Grow cache if necessary */
    if ((size_t)n >= cache_size) {
        size_t new_size = cache_size * 2;
        while (new_size <= (size_t)n) new_size <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) return 0;      /* allocation failure */
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute terms up to n if not already done */
    if ((size_t)n > max_known) {
        for (size_t i = max_known + 1; i <= (size_t)n; ++i) {
            seq_t pos = 4 * cache[i-1] + 6 * cache[i-4]
                      + 3 * cache[i-8] + 4 * cache[i-9]
                      + cache[i-11];
            seq_t neg = 4 * cache[i-2] + 3 * cache[i-3]
                      + 6 * cache[i-7] + 4 * cache[i-10];
            cache[i] = pos - neg;   /* guaranteed to be non‑negative */
        }
        max_known = (size_t)n;
    }

    return cache[(size_t)n];
}
