/*
 * A072667  Consider m such that prime(m) + prime(m+1) = prime(k) + 1 for some
 *          k; sequence gives values of prime(m).
 * https://oeis.org/A072667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 1554, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence implements a(n) = prime(m) where
   prime(m) + prime(m+1) = prime(k) + 1 for some k.
   The function searches consecutive primes and keeps the found values
   in a static dynamic array. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* undefined index, harmless */

    /* static state for memoisation */
    static uint64_t candidate = 2;            /* next number to test for prime   */
    static uint64_t prev_prime = 0;           /* previous prime encountered      */
    static seq_t *results   = NULL;           /* array of previously found terms  */
    static size_t results_len = 0;            /* number of stored terms           */
    static size_t results_cap = 0;            /* allocated capacity of array      */

    while (results_len < n)
    {
        uint64_t p;

        /* find the next prime number */
        if (candidate == 2) {
            p = 2;
            candidate = 3;
        } else {
            if ((candidate & 1) == 0)          /* skip even numbers           */
                candidate++;
            while (!is_prime(candidate))
                candidate += 2;
            p = candidate;
            candidate += 2;
        }

        /* if we have a previous prime, test the condition */
        if (prev_prime != 0) {
            uint64_t sum = prev_prime + p;     /* sum = prime(m)+prime(m+1)   */
            if (is_prime(sum - 1)) {           /* sum-1 must be prime         */
                /* extend the result array if necessary */
                if (results_len == results_cap) {
                    size_t new_cap = results_cap ? results_cap * 2 : 8;
                    seq_t *tmp = (seq_t *)realloc(results, new_cap * sizeof(seq_t));
                    if (!tmp) {
                        /* Out of memory – abort program safely */
                        exit(1);
                    }
                    results = tmp;
                    results_cap = new_cap;
                }
                results[results_len++] = (seq_t)prev_prime;
            }
        }

        prev_prime = p;                        /* prepare for next prime pair  */
    }

    return results[n - 1];
}
