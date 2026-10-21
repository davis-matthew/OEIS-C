/*
 * A084135  a(n) = 10*a(n-1) - 15*a(n-2), a(0)=1, a(1)=5.
 * https://oeis.org/A084135
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 10*a(n-1) - 15*a(n-2); a(0) = 1, a(1) = 5
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache to handle out‑of‑order calls efficiently */
    static seq_t *cache = NULL;   /* cached terms a(0)...a(k) */
    static idx_t cache_size = 0;  /* number of cached terms */

    /* initial terms for the recurrence */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;

    /* allocate or grow cache if necessary */
    if (cache == NULL) {
        /* start with capacity for two terms */
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;          /* out of memory – unlikely */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)5;
        cache_size = 2;
    }
    if (n >= cache_size) {
        /* grow cache to hold up to a(n) */
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0; /* allocation failure */
        cache = tmp;
        /* compute missing terms */
        for (idx_t i = cache_size; i <= n; ++i) {
            cache[i] = (seq_t)10 * cache[i-1] - (seq_t)15 * cache[i-2];
        }
        cache_size = new_size;
    }
    return cache[n];
}
