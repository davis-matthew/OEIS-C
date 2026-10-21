/*
 * A076051  Sum of product of odd numbers <= n and the product of even numbers
 *          <= n.
 * https://oeis.org/A076051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = product of odd ≤ n + product of even ≤ n
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 19, u64 n <= 33, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Return A076051 term for index n (offset 1). */
seq_t a(idx_t n) {
    /* Cache of odd and even products up to current maximum index. */
    static seq_t *odd_cache = NULL;   /* odd_cache[i] = product of odd numbers ≤ i */
    static seq_t *even_cache = NULL;  /* even_cache[i] = product of even numbers ≤ i */
    static idx_t cache_size = 0;      /* largest index computed so far */

    if (n == 0) return 0;  /* undefined for offset 1, but safe */

    /* Grow cache if needed. */
    if (n > cache_size) {
        /* New size includes indices 0..n */
        size_t new_size = (size_t)n + 1;
        if (cache_size == 0) {
            /* First allocation */
            odd_cache  = (seq_t *)malloc(new_size * sizeof(seq_t));
            even_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
            if (!odd_cache || !even_cache) return 0;  /* allocation failure fallback */
            odd_cache[0]  = 1;  /* product of no odd numbers = 1 */
            even_cache[0] = 1;  /* product of no even numbers = 1 */
        } else {
            odd_cache  = (seq_t *)realloc(odd_cache,  new_size * sizeof(seq_t));
            even_cache = (seq_t *)realloc(even_cache, new_size * sizeof(seq_t));
            if (!odd_cache || !even_cache) return 0;
        }

        /* Compute products from (cache_size + 1) to n */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            odd_cache[i]  = odd_cache[i-1];
            even_cache[i] = even_cache[i-1];
            if ((i & 1) == 1) {          /* odd */
                odd_cache[i] *= (seq_t)i;
            } else {                     /* even */
                even_cache[i] *= (seq_t)i;
            }
        }
        cache_size = n;
    }

    /* Return the sum of products for index n */
    return odd_cache[n] + even_cache[n];
}
