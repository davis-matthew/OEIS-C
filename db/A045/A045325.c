/*
 * A045325  Primes congruent to {0, 2, 3, 4} (mod 7).
 * https://oeis.org/A045325
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes congruent to 0, 2, 3 or 4 modulo 7.
   The implementation incrementally searches for such primes,
   stores them in a static cache, and returns the nth term.
   It works for any seq_t/idx_t definition provided by the build
   and uses the supplied seq_realloc helper for dynamic array
   resizing. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/is_prime_small.h"   /* bool is_prime_small(seq_t num) */
#include "../utils/seq_realloc.h"      /* seq_t *seq_realloc(seq_t *old, size_t old_cap, size_t new_cap) */

static seq_t *cache = NULL;          /* cache of qualifying primes */
static size_t cache_len = 0;         /* number of terms stored */
static size_t cache_cap = 0;         /* allocated capacity */
static idx_t last_checked = 1;       /* last integer that was tested */

/* Ensure the cache can hold at least `need` elements. */
static void ensure_cap(size_t need)
{
    if (cache_cap >= need)
        return;

    size_t newcap = (cache_cap == 0 ? 16 : cache_cap << 1);
    while (newcap < need)
        newcap <<= 1;

    cache = seq_realloc(cache, cache_cap, newcap);
    cache_cap = newcap;
}

seq_t a(idx_t n)
{
    /* Return cached value if already computed */
    if (n <= cache_len)
        return cache[n - 1];

    /* Start scanning from the last value we examined */
    idx_t candidate = last_checked + 1;

    while (cache_len < n) {
        if (is_prime_small((seq_t)candidate)) {
            int r = (int)(candidate % 7);
            if (r == 0 || r == 2 || r == 3 || r == 4) {
                ensure_cap(cache_len + 1);
                cache[cache_len++] = (seq_t)candidate;
            }
        }
        ++candidate;
    }

    last_checked = candidate - 1;
    return cache[n - 1];
}
