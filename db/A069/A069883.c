/*
 * A069883  Smallest nontrivial palindromic n-th perfect power (a(n) >1 for
 *          n>1).
 * https://oeis.org/A069883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  more, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   big_pow, ensure_cache, is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the smallest palindrome that is a perfect n‑th power.
   (a(1) = 1, a(2) = 4, a(3) = 8, a(4) = 14641, ...) */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/is_palindrome.h"
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    /* Minimal special cases. */
    if (n == 0) return 0;   /* undefined; just for safety */
    if (n == 1) return 1;   /* trivial 1^1 */

    /* Cache of previously computed values. */
    static seq_t *cache = NULL;
    static size_t cap = 0;

    /* Ensure the cache has room for indices up to n. */
    ensure_cache(n, &cache, &cap);

    /* Return cached result if already computed. */
    seq_t cached = cache[n - 1];
    if (cached != (seq_t)0) return cached;

    /* Search for the minimal base whose n‑th power is a palindrome. */
    seq_t best = 0;
    for (idx_t base = 2; ; ++base) {
        seq_t sb = (seq_t)base;
        seq_t pow = big_pow(sb, (seq_t)n);
        if (is_palindrome(pow, 10)) {
            best = pow;
            break;
        }
    }

    cache[n - 1] = best;
    return best;
}
