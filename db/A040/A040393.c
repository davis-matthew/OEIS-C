/*
 * A040393  Continued fraction for sqrt(414).
 * https://oeis.org/A040393
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) = n-th partial quotient of the continued fraction of
 *            sqrt(414), where a(0) = ⌊√414⌋ = 20
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: this implementation uses an incremental algorithm that keeps the
   state of the continued fraction calculation across calls so that the
   first value after a given term can be generated in O(1) time.  All values
   are computed using 64‑bit unsigned arithmetic, which is sufficient
   because every coefficient is ≤ 40.  The function is re‑entrant and
   stateless with respect to external data beyond the static cache. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Static cache of already computed partial quotients. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* allocated size of cache   */
    static idx_t  computed = 0;     /* number of terms already in cache (cache[0..computed-1]) */

    /* Static state of the continued fraction generating algorithm.  These
       correspond to the values held after the term a(computed-1). */
    static uint64_t m_state = 0;       /* m_k   */
    static uint64_t d_state = 1;       /* d_k   */
    static uint64_t a_state = 20;      /* a_k   */

    /* If we already have the requested term, return it immediately. */
    if (n < computed)
        return cache[n];

    /* Ensure the cache has enough space to store the new terms. */
    if ((size_t)(n + 1) > cache_size) {
        size_t new_size = cache_size > 0 ? cache_size * 2 : 1;
        if (new_size < (size_t)(n + 1))
            new_size = (size_t)(n + 1);

        seq_t *tmp = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failed */

        if (cache) {
            memcpy(tmp, cache, computed * sizeof(seq_t));
            free(cache);
        }
        cache = tmp;
        cache_size = new_size;
    }

    /* If this is the first call, initialise the cache with the 0th term. */
    if (computed == 0) {
        cache[0] = (seq_t)20;  /* floor(sqrt(414)) */
        computed = 1;
    }

    /* Compute terms up to n, updating the continued‑fraction state on
       each iteration. */
    const uint64_t N = 414;   /* the square‑free integer in question */
    const uint64_t a0 = 20;   /* floor(sqrt(414)) */

    for (idx_t i = computed; i <= n; ++i) {
        /* Generate next term in the continued fraction. */
        uint64_t new_m = d_state * a_state - m_state;
        uint64_t new_d = (N - new_m * new_m) / d_state;
        uint64_t new_a = (a0 + new_m) / new_d;   /* floor division */

        /* Store the new term. */
        cache[i] = (seq_t)new_a;
        computed = i + 1;

        /* Advance state for the next iteration. */
        m_state = new_m;
        d_state = new_d;
        a_state = new_a;
    }

    return cache[n];
}
