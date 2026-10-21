/*
 * A218438  G.f.: 1 / ( (1 + x^2 - x^3)^2 * (1 - x - 2*x^2 - x^3) ).
 * https://oeis.org/A218438
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=1, a(2)=1, a(3)=6, a(4)=12, a(5)=19, a(6)=48,
 *            a(7)=110, a(8)=218; Recurrence for n≥9: a(n) = a(n-1) + 5*a(n-3)
 *            + a(n-4) + a(n-5) - 3*a(n-6) - 2*a(n-7) + a(n-9)
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 29, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;     /* cached terms 0..cache_size-1 */
static idx_t cache_size = 0;    /* number of terms already cached */

static const seq_t init_values[9] = {
    (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)6, (seq_t)12,
    (seq_t)19, (seq_t)48, (seq_t)110, (seq_t)218
};

seq_t a(idx_t n) {
    /* Return cached value if already computed */
    if (n < cache_size)
        return cache[n];

    /* Extend cache to include term n */
    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) abort();            /* allocation failed */
    cache = tmp;

    /* Compute any missing terms */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i < 9) {
            cache[i] = init_values[i];
        } else {
            seq_t val = cache[i-1]
                      + 5 * cache[i-3]
                      + cache[i-4]
                      + cache[i-5]
                      - 3 * cache[i-6]
                      - 2 * cache[i-7]
                      + cache[i-9];
            cache[i] = val;
        }
    }
    cache_size = new_size;
    return cache[n];
}
