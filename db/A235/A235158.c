/*
 * A235158  Primes which have one or more occurrences of exactly six different
 *          digits.
 * https://oeis.org/A235158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 2000, u128 n <= 2000,
 *            big any n
 * Helpers:   digit_counts, ensure_capacity, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: list primes with exactly six distinct decimal digits.
   Return the nth such prime, with a(1) = 102359.  The function
   lazily generates primes, caching results so that calls in any order
   are O(n) overall. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/digit_counts.h"
#include "../utils/ensure_capacity.h"
#include "../utils/is_prime_big.h"

static seq_t *found_primes = NULL;   /* cached primes with 6 distinct digits */
static size_t found_count = 0;       /* number of cached primes */
static size_t found_cap   = 0;       /* capacity of the cache array   */
static idx_t  last_candidate = 3;    /* next odd integer to examine   */

/* Append a prime to the dynamic array, enlarging it if necessary. */
static void append_prime(seq_t p)
{
    if (found_count >= found_cap) {
        size_t new_cap = (found_cap == 0) ? 1 : found_cap * 2;
        ensure_capacity(new_cap, &found_cap, (void **)&found_primes, sizeof(seq_t));
    }
    found_primes[found_count++] = p;
}

/* Return the nth prime whose decimal representation uses exactly six
   distinct digits.  Indexing starts at 1 (a(1) = 102359). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* undefined offset, return 0 */

    /* Ensure we have computed up to the requested index. */
    while ((size_t)n > found_count) {
        /* Skip even numbers: last_candidate is always odd. */
        if ((last_candidate & 1) == 0) last_candidate++;

        if (is_prime_big((seq_t)last_candidate)) {
            unsigned char cnt[10] = {0};
            digit_counts((seq_t)last_candidate, cnt);

            unsigned distinct = 0;
            for (int d = 0; d < 10; ++d)
                if (cnt[d] > 0) distinct++;

            if (distinct == 6) {
                append_prime((seq_t)last_candidate);
            }
        }

        last_candidate += 2;   /* advance to next odd candidate */
    }

    return found_primes[(size_t)n - 1];
}
