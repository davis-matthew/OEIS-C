/*
 * A057770  The primes in A045574.
 * https://oeis.org/A057770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) is the nth prime whose decimal digits belong to the set
 *            {0,1,6,8,9}
 * Range:     u8 n <= 9, u16 n <= 174, u32 n <= 175, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_frequencies, grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

#include "../utils/is_prime_big.h"
#include "../utils/digit_frequencies.h"
#include "../utils/grow_seq.h"

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* static cache of found primes */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;

    /* candidate for testing, start from the smallest possible prime 2 */
    static seq_t candidate = 2;

    /* return cached element if already present */
    if (cache_size >= n)
        return cache[n - 1];

    /* generate primes until we have n entries */
    while (cache_size < n) {
        /* check digit set using utility */
        unsigned int freq[10] = {0};
        digit_frequencies(candidate, freq);
        if (freq[2] == 0 && freq[3] == 0 &&
            freq[4] == 0 && freq[5] == 0 &&
            freq[7] == 0) {
            /* candidate has only allowed digits */
            if (is_prime_big(candidate)) {
                /* enlarge cache if necessary */
                if (cache_size == cache_cap) {
                    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                    grow_seq(&cache, &cache_cap, new_cap);
                }
                cache[cache_size++] = candidate;
            }
        }
        /* advance to next integer */
        candidate = candidate + 1;
    }

    return cache[n - 1];
}
