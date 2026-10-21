/*
 * A075359  Sum of the digits of the next n numbers.
 * https://oeis.org/A075359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ_{k = n(n–1)/2 + 1}^{n(n+1)/2} digit_sum(k)
 * Range:     u8 n <= 22, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* that is, the digit sum of the consecutive block of integers whose
 * lengths are n (triangular numbers). */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Return the decimal digit sum of a 64‑bit integer.  */
static inline uint64_t digit_sum_uint64(uint64_t x)
{
    uint64_t s = 0;
    while (x)
    {
        s += x % 10;
        x /= 10;
    }
    return s;
}

/* Compute a(n).  The function is memoised so that arbitrary order of
 * calls is possible and the time per call is O(n) because each block
 * is processed only once.  */
seq_t a(idx_t n)
{
    /* Guard against the implicit offset 1 (a(1) is first term).  */
    if (n == 0)
        return 0;

    /* Static cache – cache[0] unused, cache[i] holds a(i).  */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* number of allocated slots (>= cache_cap) */
    static idx_t computed = 0;    /* maximum n that has been computed */

    /* Resize the cache if necessary.  Use realloc‑like logic that works
     * for arbitrary‑precision seq_t (simple assignment copy).  */
    if ((size_t)n >= cache_cap)
    {
        size_t new_cap = cache_cap > 0 ? cache_cap : 8;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;          /* double until large enough */

        /* Allocate new storage.  */
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) abort();    /* fatal in the unlikely event of OOM */

        /* Copy previously computed terms.  */
        if (cache)
        {
            for (idx_t i = 1; i <= computed; ++i)
                new_cache[i] = cache[i];
            free(cache);
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n.  */
    static uint64_t triangular = 0;   /* T_k = k(k+1)/2 previously computed */

    for (idx_t k = computed + 1; k <= n; ++k)
    {
        /* Update triangular number: T_k = T_{k-1} + k.  */
        triangular += (uint64_t)k;

        /* The block for a(k) runs from start = T_k - k + 1 to end = T_k.  */
        uint64_t start = triangular - k + 1;
        uint64_t end   = triangular;

        /* Sum digit sums of all numbers in this block.  */
        uint64_t block_sum = 0;
        for (uint64_t num = start; num <= end; ++num)
            block_sum += digit_sum_uint64(num);

        cache[k] = (seq_t)block_sum;   /* cast is safe; seq_t is an unsigned type */
    }

    computed = n;
    return cache[n];
}
