/*
 * A035092  Smallest k such that (n^2)*k + 1 is prime.
 * https://oeis.org/A035092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = smallest k such that n^2 * k + 1 is prime
 * Range:     u8 n <= 10, u16 n <= 52, u32 n <= 52, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime_big.h"

/* Compute the OEIS sequence A035092 */
seq_t a(idx_t n)
{
    /* The offset is 1, but the function works for any n >= 1. */
    if (n == 0) return 0;  /* just in case; not part of the sequence */

    /* Work in seq_t to avoid unintended truncation. */
    seq_t n_sqr = (seq_t)n * (seq_t)n;

    for (idx_t k = 1; ; ++k) {
        seq_t candidate = n_sqr * (seq_t)k + (seq_t)1;
        if (is_prime_big(candidate))
            return (seq_t)k;
    }
}
