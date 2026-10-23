/*
 * A182150  Semiprimes that are also semiprime when their digits are sorted
 *          into nondecreasing order.
 * https://oeis.org/A182150
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_seq_array, count_prime_factors, digit_frequencies,
 *            ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A182150: semiprimes that are also semiprime when their digits
 * are sorted into nondecreasing order. This implementation
 * incrementally searches integers and caches the found terms so that
 * any order of calls returns the correct n‑th value. */

#include <stddef.h>                     /* size_t */
#include "../utils/alloc_seq_array.h"   /* alloc_seq_array */
#include "../utils/count_prime_factors.h"   /* count_prime_factors */
#include "../utils/digit_frequencies.h"    /* digit_frequencies */
#include "../utils/ensure_capacity.h"      /* ensure_capacity */

seq_t a(idx_t n)
{
    /* Static cache: cache[0] = a(1). */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of terms stored */
    static size_t cache_cap  = 0;   /* allocated capacity */

    /* The next integer to test (last checked candidate). */
    static seq_t last_checked = 3;  /* initialised to 3, so first test is 4 */

    /* Ensure the cache holds at least n terms. */
    while (cache_size < n)
    {
        /* Allocate or expand the cache. */
        if (!cache)
        {
            cache_cap = 64;
            cache = alloc_seq_array(cache_cap);
        }
        else if (cache_size >= cache_cap)
        {
            size_t new_cap = cache_cap * 2;
            ensure_capacity(new_cap, &cache_cap, (void **)&cache, sizeof(seq_t));
        }

        /* Next candidate integer. */
        ++last_checked;
        const seq_t cand = last_checked;

        /* Test whether cand is a semiprime. */
        if (count_prime_factors((uint64_t)cand) == 2)
        {
            /* Compute the integer formed by sorting cand's digits */
            unsigned int freq[10] = {0};
            digit_frequencies(cand, freq);

            seq_t sorted = 0;
            for (int d = 0; d < 10; ++d)
            {
                for (unsigned int k = 0; k < freq[d]; ++k)
                {
                    sorted = sorted * 10 + d;
                }
            }

            /* Check that the sorted integer is also a semiprime. */
            if (count_prime_factors((uint64_t)sorted) == 2)
            {
                cache[cache_size++] = cand;
            }
        }
    }

    return cache[n - 1];
}
