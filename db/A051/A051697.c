/*
 * A051697  Closest prime to n (break ties by taking the smaller prime).
 * https://oeis.org/A051697
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nearest prime to n, ties broken by choosing the smaller
 *            prime
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   is_prime_small, next_prime, prev_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/prev_prime.h"
#include "../utils/next_prime.h"
#include "../utils/is_prime_small.h"

seq_t a(idx_t n) {
    /* Handle small n where 2 is the closest prime. */
    if (n < (idx_t)2) {
        return (seq_t)2;
    }

    /* If n itself is prime, it is the closest. */
    if (is_prime_small((seq_t)n)) {
        return (seq_t)n;
    }

    /* Find the previous (<= n) and next (>= n) primes. */
    idx_t p_prev = prev_prime(n);                 /* <= n */
    idx_t p_next = next_prime((uint64_t)n);       /* > n */

    /* Compute distances as seq_t to keep everything in the right type. */
    seq_t dist_prev = (seq_t)n - (seq_t)p_prev;
    seq_t dist_next = (seq_t)p_next - (seq_t)n;

    /* Ties favor the smaller prime, i.e., the previous one. */
    if (dist_prev <= dist_next) {
        return (seq_t)p_prev;
    } else {
        return (seq_t)p_next;
    }
}
