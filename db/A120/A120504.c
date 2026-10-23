/*
 * A120504  Generalized meta-Fibonacci sequence a(n) with parameters s=1 and
 *          k=3.
 * https://oeis.org/A120504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n <= 2 * n-1 if 3 <= n <= 4 * a(n-1-a(n-1)) +
 *            a(n-2-a(n-2)) + a(n-3-a(n-3)) otherwise * * This implementation
 *            memoises values in a static array. Subsequent calls * will reuse
 *            previously computed results, so any order of calls works *
 *            efficiently
 * Range:     u8 n <= 387, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache holding values for indices 1 .. current_max */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;      /* allocated size (including index 0) */
    static idx_t computed_up_to = 0;  /* highest index fully computed */

    /* enlarge cache if necessary */
    if (n + 1 > cache_size) {
        seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!new_cache) abort();      /* allocation failed */
        cache = new_cache;
        cache_size = n + 1;
    }

    /* compute any new terms up to n */
    for (idx_t i = computed_up_to + 1; i <= n; ++i) {
        if (i <= 2) {
            cache[i] = (seq_t)1;
        } else if (i <= 4) {
            cache[i] = (seq_t)(i - 1);
        } else {
            /* terms required on the right‑hand side are already computed */
            idx_t pos1 = (idx_t)(i - 1) - (idx_t)cache[i - 1];
            idx_t pos2 = (idx_t)(i - 2) - (idx_t)cache[i - 2];
            idx_t pos3 = (idx_t)(i - 3) - (idx_t)cache[i - 3];
            cache[i] = cache[pos1] + cache[pos2] + cache[pos3];
        }
    }
    computed_up_to = n;

    return cache[n];
}
