/*
 * A159698  Minimal increasing sequence beginning with 4 such that n and a(n)
 *          are either both prime or both nonprime.
 * https://oeis.org/A159698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = minimum m > a(n-1) such that m is prime iff n is prime.
 *            a(1) = 4. The sequence is memoised; each term is computed once
 *            on demand. The implementation uses the supplied prime‑test
 *            helper functions and a small dynamic allocator (grow_seq) that
 *            works correctly for both built‑in and arbitrary‑p...
 * Range:     u8 n <= 150, u16 n <= 8220, u32 n <= 8220, u64 n <= 8220,
 *            u128 n <= 8220, big any n
 * Helpers:   grow_seq, is_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"          /* bool is_prime(idx_t n);          */
#include "../utils/is_prime_big.h"      /* bool is_prime_big(seq_t n);     */
#include "../utils/grow_seq.h"           /* void grow_seq(seq_t **, size_t *, size_t); */

/* Return the n‑th term of sequence A159698 (n >= 1). */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* The sequence is defined only for n >= 1. */
        return (seq_t)0;
    }

    /* Static cache of computed terms.  cache[0] holds a(1). */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;   /* allocated size */
    static idx_t  cache_n    = 0;   /* number of terms stored */

    /* Grow the cache so that it can hold at least n elements. */
    if (cache_cap < n) {
        grow_seq(&cache, &cache_cap, n);
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = cache_n; i < n; ++i) {
        if (i == 0) {
            /* a(1) = 4 */
            cache[i] = (seq_t)4;
            continue;
        }

        seq_t prev   = cache[i - 1];
        seq_t cand   = prev + (seq_t)1;
        bool want_prime = is_prime((idx_t)(i + 1));  /* i+1 == current n */

        /* Find the next integer with the required prime property. */
        for (;;) {
            bool cand_prime = is_prime_big(cand);
            if (cand_prime == want_prime) {
                cache[i] = cand;
                break;
            }
            cand = cand + (seq_t)1;
        }
    }

    cache_n = n;
    return cache[n - 1];
}
