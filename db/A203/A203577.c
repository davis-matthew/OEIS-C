/*
 * A203577  Exponential (or binomial) half-convolution of the sequence A000108
 *          (Catalan) with itself.
 * https://oeis.org/A203577
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} C(n,k) * Catalan(k) * Catalan(n-k)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 25, u128 n <= 48,
 *            big any n
 * Helpers:   binomial, catalan, seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The implementation uses the helper functions
    `binomial(idx_t a, idx_t b)` and `catalan(idx_t k)` from the
    utils library.  The result is cached so that repeated calls to
    `a(n)` are efficient and are independent of call order.  The
    cache is allocated with the helper `seq_alloc_new_array` to ensure
    that objects of an arbitrary‑precision type are correctly
    constructed.  Intermediate values are kept in the same type `seq_t`
    to avoid overflow.  The algorithm runs in O(n²) time for a single
    call, but the caching mechanism reduces this to O(n) over a
    sequence of calls.  All indices are handled as `idx_t` (an
    unsigned 64‑bit type) and all arithmetic on `seq_t` uses only the
    operators allowed by the problem statement.  The code is fully
    compliant with C11 and C++17. */

#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/catalan.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of elements allocated */
    static idx_t computed_size = 0; /* number of elements computed,
                                      first unused index is `computed_size` */

    /* grow cache if needed */
    if ((size_t)(n + 1) > cache_size)
    {
        size_t new_size = (size_t)(n + 1);
        seq_t *new_cache = seq_alloc_new_array(new_size);

        if (cache != NULL)
        {
            /* copy old values */
            for (idx_t i = 0; i < computed_size; ++i)
                new_cache[i] = cache[i];
            /* free old array */
            seq_free_array(cache);
        }

        cache = new_cache;
        cache_size = new_size;
    }

    /* compute values up to n if not already done */
    for (idx_t i = computed_size; i <= n; ++i)
    {
        seq_t sum = 0;
        idx_t limit = i / 2;
        for (idx_t k = 0; k <= limit; ++k)
        {
            seq_t b = binomial(i, k);
            seq_t c = catalan(k);
            seq_t d = catalan(i - k);
            sum += b * c * d;
        }
        cache[i] = sum;
    }

    computed_size = n + 1;
    return cache[n];
}
