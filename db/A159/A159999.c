/*
 * A159999  Number of numbers not greater than n occurring in Collatz (3x+1)
 *          trajectory starting with n.
 * https://oeis.org/A159999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = number of integers ≤ n appearing in the 3x+1 (Collatz)
 *            trajectory that starts at n. The function is memoized so that
 *            repeated calls are fast
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* cache[n] holds a(n) for n>=1 */
    static idx_t cache_size = 0;     /* allocated size of cache (>=n+1) */

    if (n == 0) return 0;            /* value undefined below offset */

    /* grow cache if needed */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) return 0;   /* allocation failed → undefined value */
        /* initialise new entries to 0 (sentinel) */
        for (idx_t i = cache_size; i < new_size; ++i)
            new_cache[i] = 0;
        cache = new_cache;
        cache_size = new_size;
    }

    if (cache[n] != 0)                /* already computed */
        return cache[n];

    /* compute a(n) by walking the Collatz trajectory */
    uint64_t m = (uint64_t)n;         /* use 64‑bit intermediate */
    seq_t count = 0;

    for (;;) {
        if (m <= (uint64_t)n)
            count++;
        if (m == 1)
            break;
        if (m & 1U)                   /* odd → 3*m+1 */
            m = 3ULL * m + 1ULL;
        else                          /* even → m/2 */
            m >>= 1;
    }

    cache[n] = count;
    return count;
}
