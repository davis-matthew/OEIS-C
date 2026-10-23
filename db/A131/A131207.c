/*
 * A131207  Nonnegative integers n such that the difference between n and its
 *          reverse is a palindrome.
 * https://oeis.org/A131207
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth non‑negative integer m (offset 1) such that |m –
 *            rev(m)| is a palindrome in base 10
 * Range:     u8 n <= 39, u16 n <= 3675, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_palindrome, rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/rev.h"
#include "../utils/is_palindrome.h"

seq_t a(idx_t n)
{
    /* for n==0 we are outside the OEIS offset; return 0 */
    if (n == 0) return (seq_t)0;

    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of stored terms   */
    static size_t cache_cap = 0;   /* allocated size of cache  */
    static idx_t last_checked = 0; /* next m to test (inclusive) */

    /* extend the cache until it contains at least n terms   */
    while (cache_len < n) {
        idx_t m = last_checked++;          /* candidate integer       */
        idx_t rev_m = rev(m);              /* its decimal reverse     */
        idx_t diff = (rev_m >= m) ? rev_m - m : m - rev_m;

        if (is_palindrome((seq_t)diff, 10)) {
            /* append m to cache, resizing if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap == 0 ? 4 : cache_cap * 2;
                seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_cache) return (seq_t)0; /* allocation failure → fallback */
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)m;
        }
    }

    /* return the (n‑1)th element because the sequence is 1‑indexed */
    return cache[n - 1];
}
