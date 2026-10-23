/*
 * A199718  Numbers k such that 6*k-5 is composite, but 6*k-1 is prime.
 * https://oeis.org/A199718
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers k such that 6*k-5 is composite, but 6*k-1 is prime. */
/* Algorithm: iterate k starting from 1 and count satisfying values, using
   the provided is_prime_big helper. */
#include "../utils/is_prime_big.h"   /* provides: bool is_prime_big(seq_t n) */

seq_t a(idx_t n)
{
    idx_t   count = 0;   /* how many qualifying k's have been found */
    seq_t   k     = 1;   /* current candidate */

    while (1) {
        seq_t v1 = (seq_t)6 * k - 1; /* 6k-1   candidate prime */
        seq_t v2 = (seq_t)6 * k - 5; /* 6k-5   composite check */

        if (is_prime_big(v1)           /* 6k-1 is prime   */
            && v2 > 1                  /* 6k-5 > 1        */
            && !is_prime_big(v2)) {    /* 6k-5 is not prime (composite) */
            count++;
            if (count == n)            /* the n-th eligible k is found */
                return k;
        }
        ++k;                           /* try next candidate */
    }
}
