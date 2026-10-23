/*
 * A175193  a(n) is the smallest positive integer such that (the n-th
 *          prime)+a(n)! is prime, or -1 if no such prime exists.
 * https://oeis.org/A175193
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest positive integer a such that the nth prime + a!
 *            is prime, or -1 if none exists
 * Range:     u8 n <= 37, u16 n <= 6130, u32 n <= 41, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/get_prime.h"    /* uint32_t get_prime(idx_t k); */
#include "../utils/is_prime_big.h" /* bool is_prime_big(seq_t n); */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)(-1);

    /* nth prime (1‑based). */
    uint32_t p = get_prime(n);

    seq_t fact = 1;          /* factorial accumulator */
    for (idx_t a = 1; ; ++a)
    {
        if (a > 1) fact *= (seq_t)a;                 /* compute a! */
        seq_t sum = fact + (seq_t)p;                /* p + a! */
        if (is_prime_big(sum))                      /* test for primality */
            return (seq_t)a;                        /* a is the desired value */
        /* If no such a exists the loop would run forever.
           In practice for all known terms a(n) exists,
           so no explicit failure marker is required. */
    }
}
