/*
 * A049485  Primes p such that p + 510510 is also prime, where 510510 is the
 *          7th primorial number A002110(7).
 * https://oeis.org/A049485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   is_prime_big, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = the nth prime p such that p + 510510 is also prime.
   Implemented by incremental search with memoisation. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"    /* declares bool is_prime_big(seq_t) */
#include "../utils/seq_realloc.h"     /* declares seq_t *seq_realloc(...) */

static seq_t *cache      = NULL;   /* cached sequence values   */
static size_t cache_len  = 0;      /* number of cached values  */
static size_t cache_cap  = 0;      /* current capacity         */
static idx_t   last_checked = 19;  /* next candidate (odd)     */

/* Ensure at least n elements are cached. */
static void ensure_cache(idx_t n)
{
    while (cache_len < (size_t)n) {          /* need new element */
        /* search forward until a suitable prime is found */
        for (;;) {
            /* Test current candidate and the shifted value */
            if (is_prime_big((seq_t)last_checked) &&
                is_prime_big((seq_t)(last_checked + 510510))) {

                /* grow cache if necessary */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap << 1 : 8;
                    seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                    if (!tmp) abort();      /* allocation failure */
                    cache = tmp;
                    cache_cap = new_cap;
                }

                cache[cache_len++] = (seq_t)last_checked;
                last_checked += 2;          /* move to next odd candidate */
                break;                       /* ready for next element   */
            }
            last_checked += 2;               /* skip even numbers       */
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) abort();          /* OEIS offset starts at 1   */
    ensure_cache(n);
    return cache[(size_t)(n - 1)];
}
