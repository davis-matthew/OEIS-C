/*
 * A173970  Primes p such that 2*p-NextPrime are prime numbers.
 * https://oeis.org/A173970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 632, u32 n <= 48, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   free_seq_array, is_prime_big, next_prime, seq_alloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the n-th prime p such that 2*p - next_prime(p+1) is prime.
 * The function lazily generates primes using the provided next_prime() helper,
 * tests the condition with is_prime_big(), and stores found values in a
 * dynamically resizable array.  Results are memoised so that subsequent
 * calls, even out of order, can be answered in O(1).
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/next_prime.h"
#include "../utils/is_prime_big.h"
#include "../utils/seq_alloc_array.h"
#include "../utils/free_seq_array.h"

/* Note: seq_t and idx_t are defined by the build system. */

seq_t a(idx_t n)
{
    /* Memoisation data structure */
    static seq_t *results = NULL;
    static size_t results_cnt = 0;   /* number of terms stored */
    static size_t results_cap = 0;   /* allocated capacity */

    /* Prime generator state */
    static uint64_t next_to_test = 2;   /* start just before the first prime */

    /* Initialise result storage on first invocation */
    if (results == NULL) {
        results_cap = 16;
        results = seq_alloc_array(results_cap);
    }

    /* Generate terms until we have at least n of them */
    while (results_cnt < (size_t)n) {
        /* Advance to the next prime to test */
        next_to_test = next_prime(next_to_test + 1);
        uint64_t p = next_to_test;

        /* Prime immediately following p */
        uint64_t q = next_prime(p + 1);

        /* Compute r = 2*p - q; skip if not positive */
        uint64_t r_val;
        if (p + p > q) {
            r_val = p + p - q;
        } else {
            continue;   /* underflow would occur, skip this p */
        }

        /* Test whether r is prime */
        if (is_prime_big((seq_t)r_val)) {
            /* Ensure capacity for the new result */
            if (results_cnt >= results_cap) {
                size_t new_cap = results_cap * 2;
                seq_t *tmp = seq_alloc_array(new_cap);
                memcpy(tmp, results, results_cnt * sizeof(seq_t));
                free_seq_array(results);
                results = tmp;
                results_cap = new_cap;
            }
            /* Store the prime p (converted to seq_t) */
            results[results_cnt++] = (seq_t)p;
        }
    }

    /* Return the desired term (indexing starts at 1) */
    return results[n - 1];
}
