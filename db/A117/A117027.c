/*
 * A117027  Determinants of 2 X 2 matrices of non-overlapping blocks of 4
 *          consecutive primes.
 * https://oeis.org/A117027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = prime(4n-3)*prime(4n) - prime(4n-2)*prime(4n-1)
 * Range:     i8 n <= 3, i16 n <= 54, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Return the k‑th prime (k >= 1).  The primes are cached
 * incrementally in a static array.  This routine is only used
 * internally by a(x) and will be fast enough for the required
 * range (k up to ~40000 for n <= 10000). */
static uint32_t get_nth_prime(idx_t k)
{
    static uint32_t *primes   = NULL;      /* array of found primes   */
    static size_t   count    = 0;          /* number of stored primes */
    static size_t   capacity = 0;          /* allocated space        */
    static uint32_t candidate = 2;         /* candidate to test      */

    /* Generate primes until we have at least k of them. */
    while (count < k) {
        /* Test whether *candidate* is prime by trial division
         * by the primes already found.  Since *candidate* grows
         * fairly slowly we rarely need many divisions. */
        bool prime = true;
        for (size_t i = 0; i < count; ++i) {
            uint32_t p = primes[i];
            if ((uint64_t)p * p > candidate)
                break;
            if (candidate % p == 0) {
                prime = false;
                break;
            }
        }

        if (prime) {
            /* Ensure enough room for the new prime. */
            if (count == capacity) {
                capacity = capacity ? capacity * 2U : 64U;
                primes = (uint32_t *)realloc(primes, capacity * sizeof(uint32_t));
            }
            primes[count++] = candidate;
        }

        /* Next candidate: 2, then all odd numbers. */
        candidate += (candidate == 2U) ? 1U : 2U;
    }

    return primes[k - 1];
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* undefined for indices below the offset */

    /* Determine the indices of the four consecutive primes in
     * the block belonging to a(n). */
    idx_t k1 = 4 * n - 3;
    idx_t k2 = 4 * n - 2;
    idx_t k3 = 4 * n - 1;
    idx_t k4 = 4 * n;

    /* Fetch the primes. */
    uint32_t p1 = get_nth_prime(k1);
    uint32_t p2 = get_nth_prime(k2);
    uint32_t p3 = get_nth_prime(k3);
    uint32_t p4 = get_nth_prime(k4);

    /* Compute the determinant in the required integer type. */
    return (seq_t)p1 * (seq_t)p4 - (seq_t)p2 * (seq_t)p3;
}
