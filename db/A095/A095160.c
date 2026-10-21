/*
 * A095160  Let B(k) be the sum of binary digits of k. Sequence lists the
 *          numbers k such that B(k) = B(digit sum(k)).
 * https://oeis.org/A095160
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A095160:  k such that popcount(k) = popcount( digit‑sum(k) )
   Algorithm: deterministic scan of integers, caching the found terms.
   Complexity: O(max(n)) per bulk of calls. */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* Count binary 1‑bits of a seq_t.  Works for any unsigned size. */
static seq_t popcount_seq(seq_t x)
{
    seq_t c = 0;
    while (x) {
        c += x & 1;
        x >>= 1;
    }
    return c;
}

/* Sum decimal digits of a seq_t.  Result is small (≤9*digits). */
static seq_t digit_sum_seq(seq_t x)
{
    seq_t s = 0;
    while (x) {
        s += x % 10;
        x /= 10;
    }
    return s;
}

/* Return the n‑th term (0‑based) of A095160.  Terms are cached: each call
   expands the cache only as far as required.  Works for arbitrary seq_t
   (uint8_t … unsigned __int128) and idx_t is uint64_t. */
seq_t a(idx_t n)
{
    /* static cache data */
    static size_t   cache_len = 0;
    static size_t   cache_cap = 0;
    static seq_t   *cache_arr = NULL;   /* array of found terms */
    static idx_t    last_checked = 0;   /* next integer to examine */

    /* grow cache if necessary */
    if (cache_len == 0) {
        cache_cap = 256;
        cache_arr = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache_arr) exit(1);      /* out of memory */
    }

    /* expand cache until it holds at least n+1 terms */
    while (cache_len <= n) {
        /* compute term */
        seq_t k = last_checked;
        if (popcount_seq(k) == popcount_seq(digit_sum_seq(k))) {
            /* store term */
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                cache_arr = (seq_t*)realloc(cache_arr, cache_cap * sizeof(seq_t));
                if (!cache_arr) exit(1);
            }
            cache_arr[cache_len++] = k;
        }
        /* advance to next integer */
        last_checked++;
    }

    return cache_arr[n];
}
