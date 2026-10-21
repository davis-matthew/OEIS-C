/*
 * A095679  Numbers k such that k^2 + largest digit of k is prime.
 * https://oeis.org/A095679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 16, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   digit_largest, ensure_capacity, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: numbers k such that k^2 + largest digit of k is prime. */
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"
#include "../utils/ensure_capacity.h"
#include "../utils/digit_largest.h"

/* Cache of already found terms. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static idx_t last_checked = 0;

/* OEIS a(n).  n >= 1. */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset of the sequence is 1; below is undefined */
        return 0;

    /* Generate terms until the cache holds at least n entries. */
    while (cache_len < n)
    {
        ++last_checked;                         /* candidate k */
        unsigned d   = digit_largest(last_checked);
        seq_t   val  = (seq_t)last_checked * (seq_t)last_checked + (seq_t)d;

        if (is_prime_big(val))                  /* k^2 + largest digit is prime */
        {
            /* Grow the cache if necessary. */
            ensure_capacity(cache_len + 1, &cache_cap, (void **)&cache, sizeof(seq_t));
            cache[cache_len++] = (seq_t)last_checked;
        }
    }

    return cache[n - 1];
}
