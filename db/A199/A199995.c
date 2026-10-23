/*
 * A199995  Composite numbers whose multiplicative persistence is 5.
 * https://oeis.org/A199995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_product, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A199995: Composite numbers whose multiplicative persistence is 5.
   Each term is the next composite number whose multiplicative persistence equals 5.
   The function is implemented by a straightforward search with memoisation. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/digit_product.h"   /* digit_product(seq_t) */
#include "../utils/isPrimeGeneric.h" /* isPrimeGeneric(seq_t) */

static seq_t *cache = NULL;   /* cached terms */
static size_t cache_len  = 0; /* number of terms stored */
static size_t cache_cap  = 0; /* allocated capacity */
static seq_t scanning = 2;    /* next integer to test */

static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < needed)
        new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();  /* out of memory */
    cache     = tmp;
    cache_cap = new_cap;
}

/* Return true if the multiplicative persistence of n is exactly 5 */
static bool has_persistence_5(seq_t n)
{
    seq_t x = n;
    int steps = 0;
    while (x >= (seq_t)10 && steps <= 5) {
        x = digit_product(x);
        ++steps;
    }
    return steps == 5;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    size_t target = (size_t)n;
    if (cache_len < target) {
        ensure_cache_capacity(target);
        while (cache_len < target) {
            if (scanning > (seq_t)1 && !isPrimeGeneric(scanning)) {
                if (has_persistence_5(scanning))
                    cache[cache_len++] = scanning;
            }
            ++scanning;
        }
    }
    return cache[target - 1];
}
