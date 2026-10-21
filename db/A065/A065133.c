/*
 * A065133  Remainder when n-th prime is divided by the number of primes not
 *          exceeding n.
 * https://oeis.org/A065133
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n) mod π(n) (n ≥ 2)
 * Range:     u8 n <= 58, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, free_memory
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required utilities from the framework */
#include <stdint.h>
#include "../utils/build_primes_upto.h"
#include "../utils/free_memory.h"

/* External linkage function required by the solver framework */
seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 2; below values are not requested. */
    if (n < 2) {
        return 0;
    }

    /* Static state to cache generated primes between calls. */
    static uint64_t *prime_array = NULL;   /* array of primes ≤ current_limit */
    static size_t    prime_count = 0;      /* number of primes stored */
    static idx_t     current_limit = 0;    /* the largest n we have sieved to */

    /* Ensure we have at least n primes, and that all primes up to at least n are present. */
    if (prime_count < (size_t)n || current_limit < n) {
        idx_t limit = current_limit ? current_limit : 2;
        /* Double the limit until we are guaranteed enough primes. */
        while (prime_count < (size_t)n || limit < n) {
            limit *= 2;
            uint64_t *new_arr;
            size_t    new_cnt;
            /* build_primes_upto generates all primes ≤ limit. */
            build_primes_upto(limit, &new_arr, &new_cnt);
            /* Free previous array to avoid memory leaks. */
            if (prime_array) {
                free_memory(prime_array);
            }
            prime_array = new_arr;
            prime_count = new_cnt;
            current_limit = limit;
        }
    }

    /* Count how many stored primes are ≤ n. Since the array is sorted,
       a simple linear scan is fast enough for the intended limits. */
    size_t pi_n = 0;
    while (pi_n < prime_count && prime_array[pi_n] <= n) {
        ++pi_n;
    }

    /* For n ≥ 2 the count should never be 0, but guard against it. */
    if (pi_n == 0) {
        return 0;
    }

    /* nth prime is at index n-1 (array is 0‑based). */
    seq_t prime_n = (seq_t)prime_array[n - 1];

    /* Return the remainder of the division by the prime count. */
    return (seq_t)(prime_n % pi_n);
}
