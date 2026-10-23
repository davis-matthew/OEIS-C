/*
 * A127839  a(1)=1, a(2)=...=a(5)=0, a(n) = a(n-5) + a(n-4) for n > 5.
 * https://oeis.org/A127839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1, a(2) = a(3) = a(4) = a(5) = 0, a(n) = a(n-5) + a(n-4)
 *            for n > 5
 * Range:     u8 n <= 50, u16 n <= 87, u32 n <= 159, u64 n <= 302,
 *            u128 n <= 589, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

seq_t a(idx_t n)
{
    /* Guard for invalid input, defined only for n >= 1 */
    if (n == 0) return 0;

    /* Return cached value if already computed */
    if (n <= cache_size) return cache[n - 1];

    /* Grow cache to accommodate up to n */
    idx_t new_size = n;
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) return 0;  /* allocation failure: return zero */
    cache = new_cache;

    /* Initialize new entries */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        if (i == 1) {
            cache[i - 1] = 1;
        } else if (i <= 5) {
            cache[i - 1] = 0;
        } else {
            /* a(i) = a(i-5) + a(i-4); cache indices i-6 and i-5 */
            cache[i - 1] = cache[i - 6] + cache[i - 5];
        }
    }

    cache_size = new_size;
    return cache[n - 1];
}
