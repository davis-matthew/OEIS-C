/*
 * A050739  Numbers of form 9^k (values of k see A050730) containing no pair
 *          of consecutive equal digits (probably finite).
 * https://oeis.org/A050739
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 8, u64 n <= 11, u128 n <= 13,
 *            big any n
 * Helpers:   mul_small, no_consecutive_equal_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A050739: numbers 9^k (k from A050730) that contain no pair of consecutive equal digits. */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/no_consecutive_equal_digits.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* Cached results of the sequence. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static seq_t pow9 = 0;          /* current value of 9^k */
    static bool init = false;

    if (!init) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;               /* out‑of‑memory – signal failure */
        cache_len = 0;
        pow9 = (seq_t)1;                   /* 9^0 = 1 */
        init = true;
    }

    /* Produce values until we have at least (n+1) entries. */
    while (cache_len <= n) {
        if (no_consecutive_equal_digits(pow9)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) { free(cache); return 0; }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = pow9;
        }
        pow9 = mul_small(pow9, 9);
    }

    return cache[n];
}
