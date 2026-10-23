/*
 * A047160  For n >= 2, a(n) = smallest number m >= 0 such that n-m and n+m
 *          are both primes, or -1 if no such m exists.
 * https://oeis.org/A047160
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = smallest m >= 0 such that n-m and n+m are primes, or -1
 *            if no such m exists
 * Range:     u8 n <= 5130, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

/* Compute A047160 term for index n (n >= 2). */
seq_t a(idx_t n) {
    if (n < 2) return (seq_t)-1;        /* undefined below offset */
    for (idx_t m = 0; ; ++m) {
        /* n-m is the lower number; guard against underflow */
        if (m > n) break;               /* n-m < 0 impossible */
        idx_t low  = n - m;
        idx_t high = n + m;
        /* If low becomes < 2, it can never be prime for larger m. */
        if (low < 2) break;
        if (is_prime(low) && is_prime(high))
            return (seq_t)m;
        /* Avoid overflow of high on very large n. */
        if (high < n) break;
    }
    return (seq_t)-1;
}
