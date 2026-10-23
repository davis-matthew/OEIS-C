/*
 * A166685  Odd numbers that are the sum of two consecutive nonprimes.
 * https://oeis.org/A166685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A166685: sum of two consecutive non‑primes that is odd.
   The function generates non‑prime numbers (treating 0 and 1 as
   non‑prime) and sums each pair of consecutive non‑prime numbers.
   Only odd sums are stored; the n-th stored sum is returned. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n) {
    /* static cache of found odd sums */
    static seq_t *odd_sums = NULL;
    static size_t odd_cnt = 0, odd_cap = 0;

    /* state for generating non‑primes */
    static uint64_t next_check = 0;   /* next integer to test */
    static uint64_t prev_nonprime = 0;
    static bool first_pair_done = false;

    /* generate until we have at least n terms */
    while (odd_cnt < (size_t)n) {
        /* find next non‑prime */
        if (!first_pair_done) {
            /* initialise first pair 0 and 1 */
            prev_nonprime = 0;
            next_check = 1;
            first_pair_done = true;
        } else {
            /* skip primes */
            while (next_check >= 2 && is_prime((idx_t)next_check))
                next_check++;
        }

        /* current non‑prime */
        uint64_t curr_nonprime = next_check;
        next_check++;   /* prepare for next iteration */

        /* compute sum with previous non‑prime */
        seq_t sum = (seq_t)prev_nonprime + (seq_t)curr_nonprime;
        if (sum & 1) {          /* odd? */
            if (odd_cnt >= odd_cap) {
                size_t new_cap = odd_cap ? odd_cap << 1 : 4;
                seq_t *tmp = seq_realloc(odd_sums, odd_cap, new_cap);
                if (!tmp) abort();
                odd_sums = tmp;
                odd_cap   = new_cap;
            }
            odd_sums[odd_cnt++] = sum;
        }
        prev_nonprime = curr_nonprime;
    }

    return odd_sums[(size_t)n - 1];
}
