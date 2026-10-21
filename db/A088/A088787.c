/*
 * A088787  Numbers which are primes and which remain prime for three
 *          successive applications of incrementing each digit by 2 with
 *          carries ignored.
 * https://oeis.org/A088787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 17, u32 n <= 17, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes terms of A088787 by incremental search.
   For each candidate prime p, we test that p and the three
   successive digit‑transforms (digit+2 mod 10) are all prime.  The
   function keeps a cache of found terms so that calls are O(1)
   after the needed elements have been generated. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"
#if defined(__cplusplus)
# include <vector>
#endif

/* Increment each decimal digit of n by 2 modulo 10; carries are ignored. */
static inline seq_t transform_digits(seq_t n)
{
    seq_t res = 0;
    seq_t mult = 1;
    while (n > 0) {
        unsigned int d = (unsigned int)(n % 10);
        unsigned int nd = (d + 2) % 10;
        res += nd * mult;
        n   /= 10;
        mult *= 10;
    }
    return res;
}

/* Return true iff p is prime and its three successive transforms
   are also prime. */
static bool check_property(seq_t p)
{
    seq_t cur = p;
    for (int i = 0; i < 4; ++i) {
        if (!is_prime_big(cur))
            return false;
        cur = transform_digits(cur);
    }
    return true;
}

#if defined(__cplusplus)
static std::vector<seq_t> terms;      /* cache of found terms               */
static idx_t next_cand = 3;           /* next odd candidate prime         */
#else
static seq_t *terms = NULL;           /* cache on the heap                 */
static size_t terms_cap = 0;          /* allocated capacity                */
static size_t term_cnt  = 0;          /* number of cached terms            */
static idx_t next_cand = 3;           /* next odd candidate prime         */
#endif

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

#if defined(__cplusplus)
    /* Extend the cache until it contains at least n terms. */
    while (terms.size() < n) {
        while (!check_property(next_cand))
            next_cand += 2;
        terms.push_back(next_cand);
        next_cand += 2;
    }
    return terms[n - 1];
#else
    while (term_cnt < (size_t)n) {
        /* Find next candidate satisfying the property. */
        while (!check_property(next_cand))
            next_cand += 2;
        /* Ensure there is space in the cache. */
        if (term_cnt == terms_cap) {
            terms_cap = terms_cap ? terms_cap * 2 : 4;
            terms = realloc(terms, terms_cap * sizeof(seq_t));
        }
        terms[term_cnt++] = next_cand;
        next_cand += 2;
    }
    return terms[n - 1];
#endif
}
