/*
 * A040164  |First digit - last digit| for n-th prime.
 * https://oeis.org/A040164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = |first digit of the n-th prime – last digit of the n-th
 *            prime|
 * Range:     u8 n <= 54, u16 n <= 7153, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    /* Cache for the primes generated so far. */
    static uint64_t *prime_list = NULL;
    static size_t prime_count = 0;
    static idx_t current_limit = 0;

    if (n == 0) {
        return 0;               /* undefined for n < 1, but harmless */
    }

    /* Conservative upper bound for the n-th prime. */
    idx_t new_limit = (n < 6) ? (idx_t)15 : n * 15;

    /* Rebuild the sieve only when a larger n is requested. */
    if (new_limit > current_limit) {
        uint64_t *tmp_list = NULL;
        size_t tmp_cnt = 0;

        build_primes_upto(new_limit, &tmp_list, &tmp_cnt);

        /* Free previous allocation if any */
        if (prime_list) {
            free(prime_list);
        }

        prime_list = tmp_list;
        prime_count = tmp_cnt;
        current_limit = new_limit;
    }

    /* If the current list does not contain enough primes, enlarge again. */
    if (prime_count < (size_t)n) {
        /* This rarely happens because our bound is conservative, but handle it. */
        new_limit = n * 20; /* tighter bound if needed */

        uint64_t *tmp_list = NULL;
        size_t tmp_cnt = 0;

        build_primes_upto(new_limit, &tmp_list, &tmp_cnt);
        if (prime_list) {
            free(prime_list);
        }

        prime_list = tmp_list;
        prime_count = tmp_cnt;
        current_limit = new_limit;
    }

    /* Extract the n-th prime (1-based indexing). */
    uint64_t p = prime_list[n - 1];

    /* Last digit: trivial. */
    seq_t last = (seq_t)(p % 10);

    /* First digit: repeatedly divide until a single digit remains. */
    seq_t first = (seq_t)p;
    while (first >= 10) {
        first /= 10;
    }

    /* Absolute difference between first and last digits. */
    seq_t diff = (first > last) ? (first - last) : (last - first);
    return diff;
}
