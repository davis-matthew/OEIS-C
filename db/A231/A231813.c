/*
 * A231813  Number of iterations of A046665(n) = (greatest prime divisor of n)
 *          - (least prime divisor of n) [with A046665(1) = 0] required to
 *          reach zero.
 * https://oeis.org/A231813
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8913, u16 n <= 8913, u32 n <= 8913, u64 n <= 8913,
 *            u128 n <= 8913, big any n
 * Helpers:   largest_prime_divisor, smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence a(n) counts how many times you must apply
 * A046665(n) = (greatest prime divisor of n) - (least prime divisor of n)
 * until you reach 0.  Its recurrence is:
 *
 *  a(0) = 0
 *  a(n) = 1 + a(A046665(n))   for n > 0
 *
 *  Example:  a(6) = 1 + a(3−2) = 1 + a(1) = 1 + (1 + a(0)) = 2
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

#include "../utils/smallest_prime_factor.h"
#include "../utils/largest_prime_divisor.h"

/* Return a(n).  The function is memoised so that any order of calls
 * is efficient.  It works for all n that fit in idx_t (typically uint64_t). */
seq_t a(idx_t n)
{
    /* Static cache of computed values.  cache[0] is unused (a(0) = 0). */
    static seq_t *cache   = NULL;
    static size_t cache_sz = 0;

    /* Ensure the cache can hold index n.  New entries are initialised to 0. */
    if (cache == NULL || n >= cache_sz) {
        size_t new_sz = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
        if (tmp == NULL)          /* allocation failure – give up gracefully */
            return 0;
        cache = tmp;
        /* initialise new slots to 0; existing data is preserved */
        for (size_t i = cache_sz; i < new_sz; ++i)
            cache[i] = 0;
        cache_sz = new_sz;
    }

    if (n == 0)          /* a(0) = 0 */
        return 0;

    if (cache[n] != 0)   /* already computed */
        return cache[n];

    /* Base case: a(1) = 1 */
    if (n == 1) {
        cache[1] = 1;
        return 1;
    }

    /* Compute A046665(n) = greatest prime divisor – least prime divisor */
    uint64_t lpf = smallest_prime_factor((uint64_t)n);      /* least prime divisor */
    uint64_t gpf = largest_prime_divisor((uint64_t)n);      /* greatest prime divisor */
    idx_t    diff = (idx_t)(gpf - lpf);                      /* difference fits in idx_t */

    seq_t result = 1 + a(diff);               /* recursive definition */

    cache[n] = result;
    return result;
}
