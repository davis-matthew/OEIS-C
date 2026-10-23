/*
 * A103273  Number of ways of writing prime(n)-1 in the form
 *          prime(i)+prime(j).
 * https://oeis.org/A103273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered prime pairs (p_i,p_j) with p_i + p_j
 *            = prime(n)-1
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   estimate_prime_upper_bound, is_prime, simple_sieve
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* helpers from the build */
#include "../utils/simple_sieve.h"              /* void simple_sieve(uint32_t limit, uint32_t **primes_out, size_t *count_out); */
#include "../utils/is_prime.h"                 /* bool is_prime(uint64_t n); */
#include "../utils/estimate_prime_upper_bound.h" /* idx_t estimate_prime_upper_bound(idx_t n); */

/* static storage for the prime list */
static uint64_t *prime_list = NULL;
static size_t prime_cnt = 0;

/* grow the prime list so that it contains at least the first `needed` primes */
static void ensure_primes_upto(idx_t needed)
{
    if (prime_cnt >= (size_t)needed)
        return;

    /* estimate an upper bound for the nth prime */
    uint64_t lim64 = estimate_prime_upper_bound(needed);
    /* limit must fit in uint32_t for simple_sieve */
    uint32_t limit = (lim64 > UINT32_MAX) ? UINT32_MAX : (uint32_t)lim64;
    if (limit < 2)
        limit = 2;

    uint32_t *tmp = NULL;
    size_t tmp_cnt = 0;
    simple_sieve(limit, &tmp, &tmp_cnt);

    /* free old list and copy new primes into uint64_t array */
    free(prime_list);
    prime_list = (uint64_t *)malloc(tmp_cnt * sizeof(uint64_t));
    for (size_t i = 0; i < tmp_cnt; ++i)
        prime_list[i] = (uint64_t)tmp[i];
    prime_cnt = tmp_cnt;
    free(tmp);
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                      /* undefined for n==0 but safe fallback */

    ensure_primes_upto(n);

    uint64_t target = prime_list[n - 1] - 1;   /* prime(n) - 1   */
    seq_t count = 0;

    /* iterate over primes <= target/2 */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p * 2 > target)
            break;
        uint64_t q = target - p;
        if (is_prime(q))
            ++count;
    }
    return count;
}
