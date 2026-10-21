/*
 * A065885  a(n)-1, a(n) and a(n)+1 form three consecutive integers that can
 *          be factored into Fibonacci numbers.
 * https://oeis.org/A065885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1)+2*a(n-2)-a(n-3) for n > 10 Initial terms:
 *            a(1)=2, a(2)=3, a(3)=4, a(4)=5, a(5)=9, a(6)=25, a(7)=26,
 *            a(8)=64, a(9)=169, a(10)=441 The recurrence holds for all n ≥ 11
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 26, u64 n <= 49, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t base_values[10] = {
    (seq_t)2, (seq_t)3, (seq_t)4, (seq_t)5,
    (seq_t)9, (seq_t)25, (seq_t)26, (seq_t)64,
    (seq_t)169, (seq_t)441
};

seq_t a(idx_t n)
{
    /* handle indices below the offset */
    if (n <= 10) return base_values[n-1];

    /* static cache to store computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of terms currently stored */

    /* initialise cache with first ten terms if needed */
    if (!cache) {
        cache = (seq_t*)malloc(10 * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failed, return 0 */
        for (idx_t i = 0; i < 10; ++i) cache[i] = base_values[i];
        cache_size = 10;
    }

    /* grow cache if necessary */
    if (n > cache_size) {
        size_t new_size = (size_t)n;
        seq_t *new_buf = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!new_buf) return (seq_t)0; /* allocation failed */
        cache = new_buf;
        /* compute new terms up to n */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            /* i is 1‑based index, hence array positions i-1, i-2, i-3, i-4 */
            cache[i-1] = 2*cache[i-2] + 2*cache[i-3] - cache[i-4];
        }
        cache_size = n;
    }

    return cache[n-1];
}
