/*
 * A125138  a(n) = smallest m such that prime(n) divides Sum_{i=1..m} i!, or
 *          -1 if no such m exists.
 * https://oeis.org/A125138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = smallest m such that prime(n) divides sum_{i=1..m} i!, or
 *            -1 if no such m exists
 * Range:     i8 n <= 33, i16 n <= 83, i32 n <= 83, i64 n <= 83, i128 n <= 83,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Use the utility that returns the k‑th prime. */
#include "../utils/get_prime.h"
#include <stdint.h>   /* for uint64_t */

seq_t a(idx_t n)
{
    /* Retrieve the nth prime (1‑based). */
    uint32_t p = get_prime(n);   /* p fits in 32 bits for n up to 10 000 */

    /* Work modulo p using 64‑bit arithmetic to avoid overflow. */
    uint64_t P = p;
    seq_t sum  = 0;      /* current Σ i!  (mod p) */
    seq_t fact = 1;      /* current i!   (mod p) */

    /* Check values of m < p; i ranges from 1 up to p-1. */
    for (idx_t i = 1; i < p; ++i)
    {
        /* fact = fact * i (mod p) */
        uint64_t prod = (uint64_t)fact * (uint64_t)i;
        fact = (seq_t)(prod % P);

        /* sum += fact  (mod p) */
        uint64_t s = (uint64_t)sum + (uint64_t)fact;
        sum = (seq_t)(s % P);

        if (sum == (seq_t)0)   /* first m with Σ i! ≡ 0 (mod p) */
            return (seq_t)i;
    }
    /* No m < p satisfies the condition */
    return (seq_t)-1;
}
