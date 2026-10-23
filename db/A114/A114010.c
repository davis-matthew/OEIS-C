/*
 * A114010  a(1) = a(2) = 2, Let k(n) = (prime(n) + prime(n+1))/2. Then
 *          a(k(n)) = k(n). a(k(n)-i) = prime(n), a(k(n)+i) = prime(n+1) until
 *          the next prime occurs.
 * https://oeis.org/A114010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nearest prime to n (or n if there is a tie)
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    /* Special cases for small n */
    if (n <= 2) {
        return (seq_t)2;
    }

    /* If n is prime, it is its own nearest prime */
    if (is_prime((uint64_t)n)) {
        return (seq_t)n;
    }

    /* Find the nearest lower prime */
    uint64_t lower = n - 1;
    while (lower >= 2 && !is_prime(lower)) {
        lower--;
    }

    /* Find the nearest higher prime */
    uint64_t higher = n + 1;
    while (!is_prime(higher)) {
        higher++;
    }

    /* Compute distances to low and high primes */
    uint64_t dist_low  = n - lower;
    uint64_t dist_high = higher - n;

    /* If distances are equal, return n itself (tie case) */
    if (dist_low == dist_high) {
        return (seq_t)n;
    }

    /* Otherwise return the closer prime */
    if (dist_low < dist_high) {
        return (seq_t)lower;
    } else {
        return (seq_t)higher;
    }
}
