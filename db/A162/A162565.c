/*
 * A162565  Those primes p such that (p-q) divides (p-1), where q is the
 *          greatest prime < p.
 * https://oeis.org/A162565
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime p such that (p - q) divides (p - 1), where
 *            q is the greatest prime < p. (offset 1)
 * Range:     u8 n <= 33, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Requires the helper function
   uint64_t next_prime(uint64_t n)   (prime > n)                      */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    /* Memoised list of qualifying primes */
    static seq_t *list      = NULL;
    static size_t list_cap  = 0;
    static size_t list_len  = 0;

    /* The prime just before the current one (start at 2) */
    static uint64_t prev_prime = 2;
    static uint64_t cur_prime  = 2;   /* will be advanced to the first next prime */

    /* Generate entries until we have at least n of them */
    while (list_len < n) {
        /* Get the next prime after cur_prime (strictly greater) */
        cur_prime = next_prime(cur_prime);

        /* Check condition with previous prime */
        uint64_t diff = cur_prime - prev_prime;
        if (diff != 0 && (cur_prime - 1) % diff == 0U) {
            /* Grow array if needed */
            if (list_len == list_cap) {
                list_cap = list_cap ? list_cap * 2 : 16;
                list = (seq_t *)realloc(list, list_cap * sizeof(seq_t));
            }
            list[list_len++] = (seq_t)cur_prime;
        }

        /* Update for next iteration */
        prev_prime = cur_prime;
    }

    /* Return requested term (1‑based index) */
    return list[(size_t)(n - 1)];
}
