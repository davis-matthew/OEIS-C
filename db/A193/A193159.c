/*
 * A193159  Numbers having in binary representation longest palindromic
 *          subwords not longer than 3.
 * https://oeis.org/A193159
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 26, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers having in binary representation longest palindromic subwords not longer than 3.
   a(n) is the nth such natural number (a(1)=1). */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/bitlen_seq_t.h"

static seq_t *cache = NULL;          /* cached sequence values */
static size_t cache_cap = 0;         /* capacity of cache */
static idx_t cache_len = 0;          /* number of cached terms */
static seq_t next_candidate = 1;     /* next integer to test */

/* Return true if x's binary representation contains a palindrome of length > 3. */
static bool has_longer_palindrome(seq_t x)
{
    if (x == 0) return false;
    size_t len = bitlen_seq_t(x);
    if (len <= 3) return false;          /* impossible to have longer palindromes */

    for (size_t k = 4; k <= len; ++k) {   /* length of window */
        for (size_t start = 0; start + k <= len; ++start) {
            bool pal = true;
            for (size_t i = 0; i < k / 2; ++i) {
                size_t left  = start + i;
                size_t right = start + k - 1 - i;
                if ((((x >> left) & 1) != ((x >> right) & 1))) {
                    pal = false;
                    break;
                }
            }
            if (pal) return true;         /* forbidden palindrome found */
        }
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined; guard */
    while (cache_len < n) {
        if (!has_longer_palindrome(next_candidate)) {
            /* grow cache if needed */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? 2 * cache_cap : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0;          /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = next_candidate;
        }
        ++next_candidate;
    }
    return cache[n - 1];
}
