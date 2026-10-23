/*
 * A161796  Primes with at least one digit appearing exactly five times in the
 *          decimal expansion.
 * https://oeis.org/A161796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   digit_frequencies, is_prime_big, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: the n‑th prime whose decimal expansion contains a digit
   exactly five times.  Index   a(1)=101111, a(2)=111119, …  */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/is_prime_big.h"
#include "../utils/digit_frequencies.h"
#include "../utils/seq_realloc.h"

/* External access: a(n) with 1‑based index (a(0) is unused). */
seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)0;
    }

    static seq_t *cache = NULL;   /* array of qualifying primes */
    static size_t  cache_len = 0;
    static size_t  cache_cap = 0;
    static seq_t  next_candidate = 2;   /* candidate to test */

    while (cache_len < n) {
        /* Find the next qualifying prime. */
        while (1) {
            seq_t cand = next_candidate;

            if (is_prime_big(cand)) {
                unsigned int freq[10];
                digit_frequencies(cand, freq);

                bool qualifies = false;
                for (int d = 0; d < 10; ++d) {
                    if (freq[d] == 5) {
                        qualifies = true;
                        break;
                    }
                }
                if (qualifies) {
                    /* Grow the cache if necessary. */
                    if (cache_len == cache_cap) {
                        size_t new_cap = (cache_cap == 0) ? 1 : cache_cap << 1;
                        seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                        cache = tmp;
                        cache_cap = new_cap;
                    }
                    cache[cache_len++] = cand;
                    /* Advance to the next integer after the inserted prime. */
                    next_candidate = cand + (seq_t)1;
                    break;
                }
            }

            /* Advance candidate for the next trial. */
            next_candidate = cand + (seq_t)1;
        }
    }

    return cache[n - 1];
}
