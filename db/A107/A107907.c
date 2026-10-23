/*
 * A107907  Numbers having consecutive zeros or consecutive ones in binary
 *          representation.
 * https://oeis.org/A107907
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = positive integers whose binary expansion contains at
 *            least one occurrence of "00" or "11"
 * Range:     u8 n <= 247, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_consecutive_bits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has_consecutive_bits.h"

/* State for memoisation across calls. */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;
static idx_t cache_size = 0;
static unsigned long long test_num = 1;

/* Resize cache to accommodate at least n elements. */
static void ensure_cache(idx_t n)
{
    if (n <= cache_cap)
        return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 128;
    if (new_cap < n)
        new_cap = n;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr)
        abort();          /* should not happen in these tests */
    cache = new_ptr;
    cache_cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined, but safe placeholder */

    /* Extend cache until we have at least n terms. */
    ensure_cache(n);
    while (cache_size < n) {
        if (has_consecutive_bits(test_num))
            cache[cache_size++] = (seq_t)test_num;
        ++test_num;
    }
    return cache[n - 1];
}
