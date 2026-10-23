/*
 * A039786  phi(a(n)) is equal to the multiplicative projection of (a(n)-1).
 * https://oeis.org/A039786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth integer a ≥ 2 such that φ(a) equals the
 *            multiplicative projection of (a−1); a(1)=2, a(2)=3, a(3)=5,…
 * Range:     u8 n <= 34, u16 n <= 3048, u32 n <= 3049, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big, multproj_equals_m, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of 9 and all primes p for which
   multproj(p−1)=p−1, where multproj(m)=∏(p_i·e_i) over the
   prime factorisation m=∏p_i^e_i.  The code searches sequentially
   and memoises the results, storing them in a static array. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/is_prime_big.h"     /* bool is_prime_big(seq_t n);  */
#include "../utils/seq_realloc.h"     /* seq_t *seq_realloc(seq_t *p, size_t old_cap, size_t new_cap); */
#include "../utils/multproj_equals_m.h"

static seq_t *cache   = NULL;   /* memoised a(n) terms               */
static size_t cache_sz = 0;     /* number of cached terms            */
static size_t cache_cap = 0;     /* allocated capacity of cache array */

/* Extend the cache until it holds at least n terms. */
static void ensure_terms(idx_t n)
{
    if (cache_sz >= (size_t)n) return;   /* already enough terms */

    /* Grow the allocation if necessary. */
    if (cache_cap < (size_t)n)
    {
        size_t new_cap = (cache_cap ? cache_cap * 2 : 64);
        if (new_cap < (size_t)n) new_cap = n;
        seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* The next candidate to test.  */
    static seq_t cand = (seq_t)2;   /* start from 2 */

    while (cache_sz < (size_t)n)
    {
        bool ok = false;

        if (cand == (seq_t)9)
        {
            ok = true;   /* 9 is a special case */
        }
        else if (is_prime_big(cand))
        {
            uint64_t m = (uint64_t)cand - 1ULL;
            ok = multproj_equals_m(m);
        }

        if (ok)
        {
            cache[cache_sz++] = cand;
        }
        ++cand;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined below the offset */
    ensure_terms(n);
    return cache[n - 1];
}
