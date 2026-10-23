/*
 * A129621  Primes p such that at least one of k-1 and k+1 is prime, where k =
 *          sum of the two neighboring primes of 3*p.
 * https://oeis.org/A129621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   isPrimeGeneric, next_prime, prev_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A129621:
   * a(n) is the n-th prime p such that
   *   let m = 3*p,
   *   let p_left  = the largest prime < m,
   *   let p_right = the smallest prime > m,
   *   let k = p_left + p_right.
   *   At least one of k-1 or k+1 is prime.
   */

/* This implementation iterates over primes sequentially using
   the provided next_prime and prev_prime utilities.  It works
   for any build (fixed width or arbitrary precision) and
   respects the required external linkage. */

#include <stdint.h>
#include "../utils/next_prime.h"
#include "../utils/prev_prime.h"
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n) {
    /* Count of qualifying primes found so far.  Use unsigned long long
       so that count does not overflow for all required n (n up to ~10^4). */
    uint64_t count = 0;

    /* Start with the first prime 2.  The loop will process this prime
       and then advance to the next using next_prime, which returns the
       smallest prime strictly greater than its argument. */
    uint64_t p = 2;

    for (;;) {
        /* Compute 3*p; this fits in 64 bits for all p relevant to n ≤ 10000. */
        uint64_t m = 3 * p;

        /* Find neighboring primes of 3*p. */
        uint64_t p_left  = prev_prime(m);     /* largest prime < m  */
        uint64_t p_right = next_prime(m);     /* smallest prime > m */

        /* Sum of the neighbors. */
        uint64_t k = p_left + p_right;

        /* Check if k-1 or k+1 is prime.  Cast to seq_t because the
           isPrimeGeneric routine expects a seq_t argument. */
        if (isPrimeGeneric((seq_t)(k - 1)) || isPrimeGeneric((seq_t)(k + 1))) {
            ++count;
            if (count == n) {
                return (seq_t)p;   /* found the n-th term */
            }
        }

        /* Advance to the next prime.  next_prime(p) returns the next
           prime strictly greater than p. */
        p = next_prime(p);
    }
}
