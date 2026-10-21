/*
 * A227000  Numbers k such that (k+1)^2-k^2 and (k+1)^3-k^3 are both prime.
 * https://oeis.org/A227000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth integer k such that (k+1)^2-k^2 = 2k+1 and
 *            (k+1)^3-k^3 = 3k^2+3k+1 are both prime
 * Range:     u8 n <= 4, u16 n <= 19, u32 n <= 22, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   append_candidate, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/append_candidate.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* keep a cache of found elements so that calls with arbitrary n
       order are fast. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static idx_t last_k = 0;

    if (n == 0)  /* offset 1; n==0 undefined, return 0 */
        return 0;

    while (cache_len < n)
    {
        last_k++;
        seq_t k = (seq_t)last_k;

        /* 2k+1 must be prime */
        seq_t two_k_plus_one = 2 * k + 1;
        if (!is_prime_big(two_k_plus_one))
            continue;

        /* 3k^2+3k+1 must be prime */
        seq_t k_sq = k * k;
        seq_t three_k_sq_plus_three_k_plus_one = 3 * k_sq + 3 * k + 1;
        if (!is_prime_big(three_k_sq_plus_three_k_plus_one))
            continue;

        /* store k as the next term in the sequence */
        append_candidate(&cache, &cache_len, &cache_cap, k);
    }

    return cache[n - 1];
}
