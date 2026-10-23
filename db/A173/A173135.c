/*
 * A173135  Primes other than 3 and 5.
 * https://oeis.org/A173135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   is_prime, seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes excluding 3 and 5 */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

/* Returns the n-th prime number that is not 3 or 5.
   a(1) = 2, a(2) = 7, a(3) = 11, …  */
seq_t a(idx_t n)
{
    if (n == 0)        /* undefined for 0 – return 0 */
        return 0;

    /* Static cache of computed terms. */
    static seq_t *cache = NULL;
    static size_t size = 0;      /* number of cached terms      */
    static size_t capacity = 0;  /* allocated array size        */
    static idx_t last_tested = 0;/* last integer checked        */

    /* Initialise cache on first call. */
    if (size == 0) {
        capacity = 16;
        cache = seq_alloc_new_array(capacity);
        cache[0] = (seq_t)2;   /* first prime to keep */
        size = 1;
        last_tested = 2;
    }

    /* Grow the array when we need more space. */
    if (size >= capacity) {
        size_t new_cap = capacity * 2;
        seq_t *new_cache = seq_alloc_new_array(new_cap);
        for (size_t i = 0; i < size; ++i)
            new_cache[i] = cache[i];
        seq_free_array(cache);
        cache = new_cache;
        capacity = new_cap;
    }

    /* Keep generating primes until we have at least n terms. */
    while (size < n) {
        /* Advance to the next candidate, skipping 3 and 5. */
        do {
            last_tested++;
        } while (last_tested == 3 || last_tested == 5);

        if (is_prime(last_tested))
            cache[size++] = (seq_t)last_tested;
    }

    return cache[n - 1];
}
