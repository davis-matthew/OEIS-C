/*
 * A060698  The sum of the first n composite numbers minus 1 is a prime.
 * https://oeis.org/A060698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   isPrime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A060698: a(n) = the n‑th integer k for which (Σ_{i=1..k} c_i) - 1 is prime,
   where c_i are the composite numbers in increasing order.
   The implementation memorizes the sequence to answer any order of calls. */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/isPrime.h"   /* bool isPrime(idx_t x) */

seq_t a(idx_t n)
{
    /* static state preserved between calls                                     */
    static idx_t *cache = NULL;     /* cache of n values that satisfy the condition   */
    static size_t cache_len = 0;    /* number of values stored in cache             */
    static size_t cache_cap = 0;    /* current allocation capacity of cache          */
    static idx_t composites_seen = 0;  /* number of composites processed so far        */
    static idx_t sum_of_composites = 0;/* running sum of composites                   */
    static idx_t current_number = 4;   /* next candidate number (composite ?)          */

    /* build the cache up to requested term                                 */
    while (cache_len < n) {
        /* find the next composite number                                  */
        while (!isPrime(current_number)) {
            /* current_number is composite (or 1, but we start at 4)          */
            break;
        }
        /* If the current number is prime, skip it and search further.     */
        while (isPrime(current_number)) {
            ++current_number;
        }

        /* update state for this composite                                   */
        ++composites_seen;
        sum_of_composites += current_number;

        /* test the property: (sum - 1) is prime?                            */
        if (isPrime(sum_of_composites - 1)) {
            /* ensure capacity of cache array                                 */
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                idx_t *tmp = (idx_t *)realloc(cache, new_cap * sizeof(idx_t));
                if (!tmp) return 0;   /* unrecoverable allocation error   */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = composites_seen;
        }

        /* advance to next candidate number                                 */
        ++current_number;
    }

    /* value is an index of a composite, return it as seq_t                 */
    return (seq_t)cache[n - 1];
}
