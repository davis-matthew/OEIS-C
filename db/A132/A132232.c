/*
 * A132232  Primes congruent to 11 (mod 30).
 * https://oeis.org/A132232
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime congruent to 11 (mod 30)
 * Range:     u8 n <= 7, u16 n <= 816, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We generate primes of the form 11 + 30*k on demand.  
   Each time the function is called we extend the cached list
   until it contains at least n elements.  The built-in
   is_prime() routine (in ../utils/is_prime.h) is used to test
   primality.  The algorithm runs in O(n) average time for
   n up to about 10 000. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* offset is 1, so we treat n>=1; for safety return 0 for n==0 */
    if (n == 0) return 0;

    /* static state: cached primes, count, capacity and next candidate */
    static seq_t *primes = NULL;
    static idx_t count   = 0;     /* number of elements stored        */
    static idx_t capacity = 0;    /* allocated capacity of primes[]   */
    static idx_t next_candidate = 11; /* next value of the form 11+30*k to test */

    /* Grow the cache until it has at least n elements */
    while (count < n) {
        /* Find the next prime of the form 11 (mod 30) */
        while (!is_prime(next_candidate)) {
            next_candidate += 30;
        }

        /* Extend the array if necessary */
        if (count == capacity) {
            capacity = capacity == 0 ? 16 : capacity << 1;
            seq_t *tmp = (seq_t *)realloc(primes, sizeof(seq_t) * capacity);
            primes = tmp;
        }

        /* Store the found prime */
        primes[count++] = (seq_t)next_candidate;

        /* Prepare the next candidate for the next search */
        next_candidate += 30;
    }

    /* Return the n‑th element (indices start at 0 in the array) */
    return primes[n - 1];
}
