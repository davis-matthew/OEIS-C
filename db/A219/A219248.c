/*
 * A219248  Numbers such that the absolute difference of any two adjacent
 *          (decimal) digits is prime.
 * https://oeis.org/A219248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 3816, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   alloc_seq_array, grow_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th number whose absolute difference of any two adjacent
 * decimal digits is a prime (2, 3, 5 or 7).  The sequence is generated
 * by brute‑force search and cached for arbitrary‑order queries. */
#include <stdbool.h>
#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/grow_seq_array.h"

static bool is_valid(seq_t x)
{
    if (x < 10) return true;            /* single digits are always valid */
    while (x >= 10) {
        seq_t curr  = x % 10;
        seq_t next  = (x / 10) % 10;
        seq_t diff  = (curr > next) ? curr - next : next - curr;
        if (diff != 2 && diff != 3 && diff != 5 && diff != 7)
            return false;
        x /= 10;
    }
    return true;
}

seq_t a(idx_t n)
{
    static seq_t *cache     = NULL;   /* cached terms */
    static size_t cache_cap = 0;      /* cached capacity */
    static size_t cache_len = 0;      /* number of cached terms */
    static seq_t next_num   = 0;      /* next integer to test */

    /* initialise cache */
    if (cache == NULL) {
        cache_cap = 16;
        cache = alloc_seq_array(cache_cap);
    }

    /* generate terms until we have at least n of them */
    while (cache_len < n) {
        if (is_valid(next_num)) {
            if (cache_len == cache_cap)           /* grow the cache */
                grow_seq_array(&cache, &cache_cap, cache_cap * 2);
            cache[cache_len++] = next_num;
        }
        ++next_num;
    }

    return cache[(size_t)(n - 1)];
}
