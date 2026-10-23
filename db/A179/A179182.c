/*
 * A179182  Natural numbers n such that n+1 or 2n+1 is prime.
 * https://oeis.org/A179182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the n-th natural number k such that k+1 or 2k+1 is prime
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

/* Cached sequence values and state.  All indices are 1‑based in the
   OEIS sense; the first element stored is a(1).                                        */
static seq_t   *cache = NULL;   /* array of computed terms */
static size_t  cache_len = 0;   /* how many terms are actually stored */
static size_t  cache_cap = 0;   /* allocated capacity of the cache    */
static idx_t   next_candidate = 1;   /* next integer to test for the property */

/* Ensure the cache can hold at least `need` entries. */
static void
ensure_cache_cap(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < need)
        new_cap = need;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                /* allocation failure – stop execution */
    cache = tmp;
    cache_cap = new_cap;
}

seq_t
a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;           /* undefined for offset 1 – but safe */

    /* Generate terms until we have at least n of them. */
    if (cache_len < (size_t)n) {
        ensure_cache_cap((size_t)n);
        while (cache_len < (size_t)n) {
            idx_t m = next_candidate;
            bool ok = false;

            /* Test the two possible conditions.  All arithmetic is
               performed in idx_t to avoid overflow when the candidate
               is large. */
            if (is_prime(m + 1U))
                ok = true;
            else if (is_prime((m << 1) + 1U))
                ok = true;

            if (ok) {
                cache[cache_len] = (seq_t)m;
                ++cache_len;
            }
            ++next_candidate;
        }
    }

    return cache[(size_t)n - 1];
}
