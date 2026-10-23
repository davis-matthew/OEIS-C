/*
 * A033236  Primes of form x^2+57*y^2.
 * https://oeis.org/A033236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes p of the form x^2+57*y^2 ⇔ p ≡
 *            1,25,49,61,73,85,121,157,169 (mod 228)
 * Range:     u8 n <= 4, u16 n <= 803, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, is_good_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"   /* declares build_primes_upto */
#include "../utils/is_good_prime.h"

static seq_t *pval = NULL;      /* cached good primes */
static size_t pval_cnt = 0;      /* number of cached values   */
static idx_t  pval_limit = 0;    /* prime sieve upper limit used for the cache */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* offset is 1; undefined for n=0, return 0 */

    /* If already cached, return it */
    if (n <= pval_cnt)
        return pval[n - 1];

    /* Figure out a new upper bound for the prime sieve */
    idx_t new_limit = n * 210ull + 1000ull;          /* safety margin */
    if (new_limit <= pval_limit)
        new_limit = pval_limit * 2 + 1000ull;        /* double if needed */

    /* Generate all primes up to new_limit */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(new_limit, &primes, &prime_cnt);

    /* Allocate a cache big enough for all primes found */
    seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * prime_cnt);
    size_t cnt = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        /* The smallest prime in the sequence is 61 */
        if (p >= 61 && is_good_prime(p))
            new_cache[cnt++] = (seq_t)p;
    }

    /* Free old resources */
    free(pval);
    free(primes);

    /* Replace old cache */
    pval = new_cache;
    pval_cnt = cnt;
    pval_limit = new_limit;

    /* If still not enough primes, recurse to enlarge cache */
    if (n > pval_cnt)
        return a(n);
    return pval[n - 1];
}
