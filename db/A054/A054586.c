/*
 * A054586  a(n) = Sum_{d|2n+1} phi(d)*mu(d).
 * https://oeis.org/A054586
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = \prod_{p | (2*n+1)} (2 - p)
 * Range:     i8 n <= 60, i16 n <= 60, i32 n <= 60, i64 n <= 60, i128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (product over distinct prime divisors p of 2*n+1; empty product = 1) */
#include <stdint.h>
#include <stdbool.h>

/* Computes a(n) = Sum_{d | (2*n+1)} phi(d)*mu(d) 
   which simplifies to the product of (2 - p) for each distinct prime p dividing 2*n+1. */
seq_t a(idx_t n)
{
    /* 2*n+1 may overflow 64 bits for very large n, but the test suite only uses n up to ~10^4.
       We compute the product using seq_t which can hold the result. */
    uint64_t m = 2ULL * n + 1ULL;          /* 2*n+1 fits in uint64_t for n in test range */
    seq_t prod = 1;                        /* empty product = 1 */

    /* trial division over odd factors (m is odd) */
    uint64_t p = 3;
    while (p * p <= m) {
        if (m % p == 0) {
            /* p is a distinct prime divisor */
            prod *= ((seq_t)2 - (seq_t)p);
            while (m % p == 0) m /= p;    /* remove all powers of p */
        }
        p += 2;                            /* skip even numbers */
    }

    /* If remaining m > 1, it is a prime factor > sqrt(original m). */
    if (m > 1) {
        prod *= ((seq_t)2 - (seq_t)m);
    }

    return prod;
}
