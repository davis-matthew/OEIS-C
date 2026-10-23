/*
 * A184593  5n - A101306: sum_{i=1..n} the last digit of prime(i).
 * https://oeis.org/A184593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, sign
 * Formula:   a(n) = 5*n - Sum_{i=1..n} (Prime(i) mod 10)
 * Range:     i8 n <= 80, i16 n <= 80, i32 n <= 80, i64 n <= 80, i128 n <= 80,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* static state for incremental computation */
    static uint32_t *primes = NULL;       /* prime list      */
    static size_t   prime_cap = 0;        /* capacity         */
    static size_t   prime_cnt = 0;        /* number of primes */

    static seq_t *cum = NULL;            /* cumulative sums of last digits */
    static size_t  cum_cap = 0;          /* capacity of cum array   */
    static size_t  cum_len = 0;          /* number of computed terms */

    static idx_t next_candidate = 2;     /* next number to test */

    if (n == 0)            /* offset is 1, so this case is unused */
        return 0;

    /* keep generating primes until we have at least n of them */
    while (cum_len < n) {
        idx_t cand = next_candidate;

        if (is_prime(cand)) {
            /* store the new prime */
            if (prime_cnt == prime_cap) {
                prime_cap = prime_cap ? prime_cap * 2 : 16;
                uint32_t *tmp = (uint32_t *)realloc(primes,
                                                    prime_cap * sizeof(uint32_t));
                if (!tmp) abort();
                primes = tmp;
            }
            primes[prime_cnt++] = (uint32_t) cand;

            /* update cumulative sum of last digits */
            seq_t digit      = (seq_t)(cand % 10);
            seq_t prev_sum   = cum_len ? cum[cum_len - 1] : 0;
            seq_t new_sum    = prev_sum + digit;

            if (cum_len == cum_cap) {
                cum_cap = cum_cap ? cum_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cum,
                                              cum_cap * sizeof(seq_t));
                if (!tmp) abort();
                cum = tmp;
            }
            cum[cum_len++] = new_sum;
        }

        ++next_candidate;
    }

    /* compute a(n) = 5*n - cumulative_last_digit_sum(n) */
    seq_t sum_last = cum[n - 1];
    seq_t res = 5 * (seq_t)n - sum_last;
    return res;
}
