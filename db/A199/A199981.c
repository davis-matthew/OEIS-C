/*
 * A199981  Composite numbers whose multiplicative digital root is 2.
 * https://oeis.org/A199981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   digit_product, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A199981: composite numbers whose multiplicative digital root is 2.
   The function builds the sequence lazily and memoises the results, so
   that repeated calls with arbitrary n are answered by lookup. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/digit_product.h"

static seq_t *cache = NULL;      /* cache[i-1] holds a(i) for i >= 1 */
static size_t cache_cap = 0, cache_len = 0;
static idx_t next_candidate = 2; /* integer from which we test membership */

/* Ensure the cache can hold at least `n` elements. */
static void ensure_cache(size_t n)
{
    if (cache_cap < n) {
        size_t new_cap = cache_cap ? (cache_cap << 1) : 16;
        while (new_cap < n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Return true iff the multiplicative digital root of `x` equals 2. */
static bool mdr_is_two(idx_t x)
{
    seq_t v = (seq_t)x;
    while (v >= (seq_t)10) v = digit_product(v);
    return v == (seq_t)2;
}

/* a(n): return the n-th member of A199981 (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* out of range */

    while (cache_len < (size_t)n) {
        /* Find the next qualifying composite number. */
        while (true) {
            if (next_candidate >= (idx_t)4 &&            /* composites start at 4 */
                !is_prime(next_candidate) &&            /* composite */
                mdr_is_two(next_candidate)) {           /* MDR == 2 */
                ensure_cache(cache_len + 1);
                cache[cache_len++] = (seq_t)next_candidate;
                ++next_candidate; /* move past the found number */
                break;
            }
            ++next_candidate;
        }
    }
    return cache[(size_t)n - 1];
}
