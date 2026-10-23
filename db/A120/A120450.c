/*
 * A120450  Number of ways to express a prime p as 2*p1 + 3*p2, where p1, p2
 *          are primes or 1.
 * https://oeis.org/A120450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of representations of the n-th prime p as
 *            2*p1 + 3*p2, where p1 and p2 are primes or 1
 * Range:     u8 n <= 272, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _prime_limit_for_idx, build_primes_upto, get_prime,
 *            is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/get_prime.h"
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    static uint64_t *prime_list = NULL;
    static size_t prime_cnt   = 0;
    static uint64_t prime_lim = 0;
    static idx_t   max_n_seen = 0;

    if (n < 1)
        return 0;

    /* Update prime table if a larger index is requested */
    if (n > max_n_seen) {
        uint64_t lim = _prime_limit_for_idx(n);
        if (prime_list) free(prime_list);
        build_primes_upto(lim, &prime_list, &prime_cnt);
        prime_lim = lim;
        max_n_seen = n;
    }

    /* Get the n-th prime */
    uint64_t p = get_prime(n);

    seq_t count = 0;

    /* Check pair where p2 == 1 */
    {
        uint64_t temp = p - 3ULL;         /* 3*1 */
        if (temp > 0 && (temp & 1ULL) == 0) {
            uint64_t p1 = temp >> 1;      /* (p - 3)/2 */
            if (p1 == 1ULL || is_prime_small(p1))
                ++count;
        }
    }

    /* Iterate over prime p2 values */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p2 = prime_list[i];
        if (p2 > p / 3ULL) break;         /* 3*p2 must be <= p */

        uint64_t temp = p - 3ULL * p2;
        if (temp == 0 || (temp & 1ULL) == 1ULL)
            continue;                    /* cannot be even or non-positive */

        uint64_t p1 = temp >> 1;          /* (p - 3*p2)/2 */
        if (p1 == 1ULL || is_prime_small(p1))
            ++count;
    }

    return count;
}
