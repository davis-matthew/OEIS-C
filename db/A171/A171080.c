/*
 * A171080  a(n) = Product_{3 <= p <= 2*n+1, p prime} p^floor(2*n / (p - 1)).
 * https://oeis.org/A171080
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   big_pow, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A171080
    Formula: a(n) = ∏_{3 ≤ p ≤ 2n+1 , p prime} p^{⌊2n/(p−1)⌋}
    (the prime 2 is excluded from the product)
*/
#include <stdlib.h>
#include "../utils/big_pow.h"
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    /* Cache of primes up to the largest limit seen so far */
    static idx_t cached_limit = 0;
    static size_t cached_cnt   = 0;
    static uint64_t *cached_pr = NULL;

    /* Compute the upper bound for the primes to include */
    idx_t two_n = n + n;          /* 2 * n  (using idx_t to avoid overflow) */
    idx_t limit = two_n + 1;      /* 2 * n + 1 */

    /* Rebuild the prime cache if the current limit is larger than cached */
    if (limit > cached_limit)
    {
        /* Release previous allocation if any */
        if (cached_pr)
            free(cached_pr);

        /* build_primes_upto returns array of primes ≤ limit and its count */
        build_primes_upto(limit, &cached_pr, &cached_cnt);

        cached_limit = limit;
    }

    seq_t result = 1;  /* multiplicative identity */

    /* Multiply over all primes ≥ 3 in the cached list */
    for (size_t i = 0; i < cached_cnt; ++i)
    {
        uint64_t p = cached_pr[i];
        if (p < 3)            /* skip the prime 2 */
            continue;
        if ((idx_t)p > limit) /* safety check, though should not happen */
            break;

        /* exponent = floor(2n/(p−1)) */
        uint64_t exp = two_n / (p - 1);
        if (exp == 0)
            continue;

        result *= big_pow((seq_t)p, (seq_t)exp);
    }

    return result;
}
