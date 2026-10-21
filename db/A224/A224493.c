/*
 * A224493  Smallest k such that k*2*p(n)^2+1 is prime.
 * https://oeis.org/A224493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest k ≥ 1 such that k*2*p(n)^2 + 1 is prime,
 *            where p(n) is the n‑th prime. For n = 0 the function returns 0
 *            (offset not defined). The function memoizes results, so calls in
 *            any order or repeated indices remain fast
 * Range:     u8 n <= 4, u16 n <= 15, u32 n <= 1305, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, is_prime_big, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime_small.h"
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;    /* memoized sequence values */
static size_t  cache_cap = 0;  /* allocated size of cache            */
static idx_t   cache_used = 0; /* highest n that has been computed   */

/* helper: test primality of cand, choosing the fastest routine for
   the current seq_t size. */
static bool is_prime_candidate(seq_t cand)
{
    if (sizeof(seq_t) <= 8) {
        return is_prime_small((uint64_t)cand);
    } else {
        return is_prime_big(cand);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* offset 1 is the first defined term */

    /* fast lookup if already computed */
    if (cache && n <= cache_used) {
        return cache[(size_t)n - 1];
    }

    /* grow cache if needed */
    if (n > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort(); /* memory allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute p(n) = n-th prime */
    uint32_t p = get_prime((uint32_t)n);

    /* compute m = 2 * p^2  using a 128‑bit intermediate if seq_t is
       not arbitrary‑precision, to avoid overflow. */
    seq_t m;
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 tmp = (unsigned __int128)2 * p * p;
        m = (seq_t)tmp;
    } else {
        m = (seq_t)2 * (seq_t)p * (seq_t)p;
    }

    /* search for the smallest k such that m*k+1 is prime */
    idx_t k = 1;
    for (;;) {
        unsigned __int128 prod = (unsigned __int128)m * (unsigned __int128)k;
        seq_t cand = (seq_t)(prod + 1);
        if (is_prime_candidate(cand)) {
            seq_t kseq = (seq_t)k;
            cache[(size_t)n - 1] = kseq;
            if (n > cache_used) cache_used = n;
            return kseq;
        }
        ++k;
    }
}
