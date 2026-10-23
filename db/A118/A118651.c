/*
 * A118651  Numbers k such that k^2 is a palindrome when written in base 17.
 * https://oeis.org/A118651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = k where k^2 is a palindrome when written in base 17 (OEIS
 *            A118651). The function returns the n-th term (a(1) = 0)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 51, u64 n <= 100, u128 n <= 100,
 *            big any n
 * Helpers:   grow_seq, is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation performs an incremental search and caches results for
   repeated calls.  It uses the supplied is_palindrome utility and a
   generic grow_seq helper to manage the cache. */
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_palindrome.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    static seq_t *cache     = NULL;   /* cache of found k values   */
    static size_t cache_cap = 0;      /* allocated capacity       */
    static size_t cache_len = 0;      /* number of elements in cache */
    static idx_t  next_k    = 0;      /* next k to test            */

    /* Extend cache until we have at least n elements */
    while (cache_len < (size_t)n) {
        seq_t kseq = (seq_t)next_k;
        seq_t sq   = kseq * kseq;           /* k²  */
        if (is_palindrome(sq, (idx_t)17)) {
            grow_seq(&cache, &cache_cap, cache_len + 1);
            cache[cache_len++] = kseq;
        }
        ++next_k;
    }

    return cache[(size_t)n - 1];
}
