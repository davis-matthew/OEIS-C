/*
 * A049653  a(n) = 2*n - prevprime(2*n).
 * https://oeis.org/A049653
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n - prevprime(2*n)
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   prev_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/prev_prime.h"

seq_t a(idx_t n)
{
    /* Sequence defined for n ≥ 2; for smaller n return 0. */
    if (n < 2) return 0;

    /* Compute 2*n using idx_t to keep the value in the index type. */
    idx_t two_n = n + n;         /* 2*n */
    idx_t p = prev_prime(two_n); /* largest prime ≤ 2*n */

    /* Perform the arithmetic in seq_t. */
    return ((seq_t)n * 2) - (seq_t)p;
}
