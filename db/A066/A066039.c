/*
 * A066039  Largest prime less than or equal to the sum of first n primes
 *          (A007504).
 * https://oeis.org/A066039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   largest prime <= sum of first n primes, OEIS A066039
 * Range:     u8 n <= 13, u16 n <= 156, u32 n <= 200, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_capacity, is_prime_big, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/is_prime_small.h"
#include "../utils/is_prime_big.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    if (n == 0)  /* undefined for n<1, return 0 to avoid crash */
        return 0;

    /* Static state */
    static seq_t *sum_arr  = NULL;   /* cumulative sums of primes   */
    static uint64_t *prime_arr = NULL; /* list of primes              */
    static size_t sum_cap = 0;
    static size_t prime_cap = 0;
    static size_t sum_cnt = 0;   /* number of sums computed (=number of primes) */
    static size_t prime_cnt = 0;

    /* Fill tables up to requested n */
    while (sum_cnt < (size_t)n)
    {
        /* Find next prime */
        uint64_t cand = 2;
        if (prime_cnt > 0)
            cand = prime_arr[prime_cnt - 1] + 1;

        while (!is_prime_small(cand))
            ++cand;

        /* Store prime */
        if (prime_cnt == prime_cap) {
            size_t new_cap = prime_cap ? prime_cap * 2 : 8;
            ensure_capacity(new_cap, &prime_cap, (void **)&prime_arr,
                            sizeof(uint64_t));
        }
        prime_arr[prime_cnt++] = cand;

        /* Store cumulative sum */
        seq_t new_sum = (sum_cnt == 0) ? (seq_t)cand
                                       : sum_arr[sum_cnt - 1] + (seq_t)cand;
        if (sum_cnt == sum_cap) {
            size_t new_cap = sum_cap ? sum_cap * 2 : 8;
            ensure_capacity(new_cap, &sum_cap, (void **)&sum_arr, sizeof(seq_t));
        }
        sum_arr[sum_cnt++] = new_sum;
    }

    /* Find largest prime <= the sum */
    seq_t target = sum_arr[n - 1];
    seq_t result = target;

    while (!is_prime_big(result))
        result--;

    return result;
}
