/*
 * A045365  Primes congruent to {2, 5, 6} mod 7.
 * https://oeis.org/A045365
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes congruent to 2, 5 or 6 (mod 7)
 * Range:     u8 n <= 27, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        /* behavior below the offset doesn't matter */
        return 0;
    }

    /* static tables holding all primes found so far,
       and only the primes in the desired residue classes */
    static uint64_t *all       = NULL;
    static size_t   all_cnt    = 0;
    static size_t   all_cap    = 0;

    static seq_t *selected     = NULL;
    static size_t   selected_cnt = 0;
    static size_t   selected_cap = 0;

    /* next integer to test for primality (start at the smallest prime) */
    static uint64_t next_candidate = 2;

    /* build the sequence until it contains at least n elements */
    while (selected_cnt < (size_t)n) {

        uint64_t candidate = next_candidate;

        /* primality test using the list of primes already found */
        bool is_prime = true;
        for (size_t i = 0; i < all_cnt; ++i) {
            uint64_t p = all[i];
            if (p * p > candidate)
                break;
            if (candidate % p == 0) {
                is_prime = false;
                break;
            }
        }

        /* store the new prime */
        if (is_prime) {
            if (all_cnt == all_cap) {
                size_t new_cap = all_cap ? all_cap << 1 : 64;
                uint64_t *tmp   = (uint64_t *)realloc(all, new_cap * sizeof(uint64_t));
                if (!tmp) abort();
                all    = tmp;
                all_cap = new_cap;
            }
            all[all_cnt++] = candidate;

            /* keep the primes that satisfy the 7‑mod condition */
            uint64_t r = candidate % 7U;
            if (r == 2U || r == 5U || r == 6U) {
                if (selected_cnt == selected_cap) {
                    size_t new_cap = selected_cap ? selected_cap << 1 : 64;
                    seq_t *tmp     = (seq_t *)realloc(selected, new_cap * sizeof(seq_t));
                    if (!tmp) abort();
                    selected   = tmp;
                    selected_cap = new_cap;
                }
                selected[selected_cnt++] = (seq_t)candidate;
            }
        }

        /* advance to the next candidate:
           skip even numbers greater than 2 */
        if (candidate == 2)
            next_candidate = 3;
        else
            next_candidate += 2;
    }

    return selected[n - 1];
}
