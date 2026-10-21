/*
 * A233080  Number of n X 6 0..3 arrays with no element x(i,j) adjacent to
 *          value 3-x(i,j) horizontally or antidiagonally, top left element
 *          zero, and 1 appearing before 2 in row major order.
 * https://oeis.org/A233080
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 283*a(n-1) - 4251*a(n-2) + 13573*a(n-3) - 9604*a(n-4);
 *            Base values: a(1)=122, a(2)=32414, a(3)=8662343, a(4)=2315298290
 *            The sequence satisfies this order‑4 linear recurrence for all n
 *            ≥ 5
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Constants for the first four terms */
    static const seq_t init[4] = {122, 32414, 8662343, 2315298290};

    /* Cached computed values – grow as needed */
    static seq_t *cache = NULL;
    static idx_t cached_n = 0;   /* already computed up to cache[cached_n-1] */

    /* Handle the base cases directly */
    if (n <= 4) {
        return init[n - 1];
    }

    /* Initialize cache with the first four values if needed */
    if (cache == NULL) {
        cache = (seq_t *)malloc(4 * sizeof(seq_t));
        if (!cache) return 0;  /* allocation failure, unlikely for n ≤ 4 */
        for (idx_t i = 0; i < 4; i++) cache[i] = init[i];
        cached_n = 4;
    }

    /* If the needed value is already cached, return it */
    if (n <= cached_n) {
        return cache[n - 1];
    }

    /* Extend the cache up to n */
    size_t new_size = (size_t)n;
    seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
    if (!new_cache) return 0;  /* allocation failure */

    /* Copy existing cached values */
    for (size_t i = 0; i < cached_n; i++) {
        new_cache[i] = cache[i];
    }

    /* Compute remaining terms using the recurrence */
    for (size_t i = cached_n; i < new_size; i++) {
        seq_t t1 = (seq_t)283  * new_cache[i - 1];
        seq_t t2 = (seq_t)4251 * new_cache[i - 2];
        seq_t t3 = (seq_t)13573 * new_cache[i - 3];
        seq_t t4 = (seq_t)9604 * new_cache[i - 4];
        new_cache[i] = t1 + t3 - t2 - t4;
    }

    /* Replace old cache with the new one */
    free(cache);
    cache = new_cache;
    cached_n = (idx_t)new_size;

    return cache[n - 1];
}
