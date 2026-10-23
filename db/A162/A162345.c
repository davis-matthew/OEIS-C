/*
 * A162345  Length of n-th edge in the graph of the zig-zag function for prime
 *          numbers.
 * https://oeis.org/A162345
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (prime(n+1) - prime(n-1)) / 2 for n >= 3, with a(1) =
 *            a(2) = 2
 * Range:     u8 n <= 53, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n) {
    if (n <= 2) {                /* base cases from the OEIS data */
        return (seq_t)2;
    }

    /* Retrieve the required primes.  get_prime returns the k-th prime
       (1‑based).  The result fits into a 32‑bit unsigned integer.  We
       cast to seq_t before subtraction to avoid any signed overflow. */
    seq_t p_next = (seq_t)get_prime(n + 1);   /* prime(n+1) */
    seq_t p_prev = (seq_t)get_prime(n - 1);   /* prime(n-1) */

    /* Compute the difference and divide by two. */
    return (p_next - p_prev) / (seq_t)2;
}
