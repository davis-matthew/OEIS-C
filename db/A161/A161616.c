/*
 * A161616  Primes p such that 2*p+111546435 is also prime.
 * https://oeis.org/A161616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   primes p such that 2*p + 111546435 is also prime
 * Range:     u8 none, u16 none, u32 none, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   is_prime_big, is_prime_small, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_small.h"
#include "../utils/is_prime_big.h"
#include "../utils/seq_realloc.h"

/* Memoised cache for the sequence terms. */
static seq_t *cache      = NULL;   /* array of found primes p */
static size_t cache_len  = 0;      /* number of stored terms */
static size_t cache_cap  = 0;      /* capacity of the array */

/* Next odd candidate to test; 2 is handled separately. */
static idx_t next_candidate = 2;

/* Return the n-th term of A161616 (1‑based). */
seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined behaviour for n<1, return 0 */

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        /* Test candidate for primality. */
        if (next_candidate == 2) {
            /* 2 is the only even prime. */
            if (is_prime_small((seq_t)2)) {
                seq_t q = ((seq_t)2) * 2 + (seq_t)111546435;
                if (is_prime_big(q)) {
                    seq_t *tmp = seq_realloc(cache, cache_cap, cache_len + 1);
                    if (!tmp) return 0;   /* allocation failed */
                    cache = tmp;
                    cache_cap = cache_len + 1;
                    cache[cache_len] = (seq_t)2;
                    cache_len++;
                }
            }
            next_candidate = 3;          /* next candidate will be odd */
        } else {
            /* Only test odd numbers. */
            seq_t cand = (seq_t)next_candidate;
            if (is_prime_small(cand)) {
                seq_t q = cand * 2 + (seq_t)111546435;
                if (is_prime_big(q)) {
                    seq_t *tmp = seq_realloc(cache, cache_cap, cache_len + 1);
                    if (!tmp) return 0;   /* allocation failed */
                    cache = tmp;
                    cache_cap = cache_len + 1;
                    cache[cache_len] = cand;
                    cache_len++;
                }
            }
            /* Advance to next odd integer. */
            next_candidate += 2;
        }
    }

    /* Return the cached value. */
    return cache[n - 1];
}
