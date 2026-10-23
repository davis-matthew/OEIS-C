/*
 * A174100  Numbers k such that both 2*k + 1 and 6*k + 1 are prime.
 * https://oeis.org/A174100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth integer k (k ≥ 1) such that 2*k+1 and 6*k+1 are
 *            prime
 * Range:     u8 n <= 13, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   is_prime_big, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_realloc.h" /* seq_realloc(seq_t*, size_t, size_t) */
#include "../utils/is_prime_big.h" /* bool is_prime_big(seq_t) */

seq_t a(idx_t n)
{
    /* a(1) is the first term.  The function is defined for n ≥ 1.  */
    if (n == 0) return 0;  /* defensive, not used for valid offset */

    /* Static memoised cache of found k’s.  */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;   /* how many terms are stored */
    static size_t cache_cap  = 0;   /* capacity of the cache array */
    static idx_t  last_k     = 0;   /* highest k that has been examined */

    /* Allocate or grow the cache array when needed.  */
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
        cache = seq_realloc(cache, cache_cap, new_cap);
        cache_cap = new_cap;
    }

    /* Generate new terms until the cache contains at least n entries.  */
    if (cache_len < (size_t)n) {
        for (idx_t k = last_k + 1; cache_len < (size_t)n; ++k) {
            last_k = k;
            seq_t sk = (seq_t)k;
            seq_t t1 = sk * 2 + 1;
            seq_t t2 = sk * 6 + 1;
            if (is_prime_big(t1) && is_prime_big(t2)) {
                cache[cache_len++] = sk;
            }
        }
    }

    /* Return the nth term (1‑based).  */
    return cache[n - 1];
}
