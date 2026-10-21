/*
 * A073249  Nonprime squarefree numbers n such that both n-1 and n+1 are not
 *          squarefree and not prime.
 * https://oeis.org/A073249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for each term by testing integers for the defined conditions */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/is_squarefree.h"

/* seq_t a(idx_t n)
   Returns the n-th nonprime square‑free number n such that
   both n-1 and n+1 are composite and not square‑free. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1 */

    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static uint64_t last_checked = 1; /* start from 2 */

    while (cache_len < n) {
        ++last_checked;               /* next candidate */

        seq_t cand = (seq_t)last_checked;
        if (is_prime_big(cand)) continue;          /* skip primes */
        if (!is_squarefree(cand)) continue;        /* need square‑free */

        /* neighbor n-1 */
        seq_t nm1 = (seq_t)(last_checked - 1);
        if (is_prime_big(nm1) || is_squarefree(nm1)) continue;

        /* neighbor n+1 */
        seq_t np1 = (seq_t)(last_checked + 1);
        if (is_prime_big(np1) || is_squarefree(np1)) continue;

        /* valid term found, add to cache */
        if (cache_len == 0) {
            cache_cap = 16;
            cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        } else if (cache_len == cache_cap) {
            cache_cap *= 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[cache_len++] = cand;
    }
    return cache[n-1];
}
