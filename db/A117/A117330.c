/*
 * A117330  a(n) is the determinant of the 3 X 3 matrix with entries the 9
 *          consecutive primes starting with the n-th prime.
 * https://oeis.org/A117330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) is the determinant of the 3x3 matrix whose entries are the
 *            9 consecutive primes starting with the n-th prime
 * Range:     i8 n <= 7, i16 n <= 95, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes for prime generation and standard utilities */
#include <stdlib.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"

/* External linkage as required */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* undefined for n < 1, return 0 */

    /* Static storage for primes */
    static uint64_t *prime_array = NULL;
    static size_t      prime_cnt   = 0;
    static idx_t       prime_limit = 200000;   /* initial limit, grows if needed */

    /* Build primes on first call or if more are needed */
    if (prime_array == NULL) {
        build_primes_upto(prime_limit, &prime_array, &prime_cnt);
    }
    /* Ensure we have at least (n+8) primes (the 9-th after the n-th) */
    while ((idx_t)(prime_cnt) <= n + 7) {
        prime_limit *= 2;                    /* double the sieve limit */
        free(prime_array);                   /* free the old array */
        build_primes_upto(prime_limit, &prime_array, &prime_cnt);
    }

    /* Load the 9 consecutive primes starting at index n-1 (0‑based) */
    seq_t p[9];
    for (idx_t i = 0; i < 9; ++i)
        p[i] = (seq_t)prime_array[n - 1 + i];

    /* Determinant of a 3x3 matrix using the rule of Sarrus
       |p0 p1 p2|   |p3 p4 p5|   |p6 p7 p8|
       determinant = p0*(p4*p8 - p5*p7)
                     - p1*(p3*p8 - p5*p6)
                     + p2*(p3*p7 - p4*p6) */
    seq_t term1 = p[0] * (p[4] * p[8] - p[5] * p[7]);
    seq_t term2 = p[1] * (p[3] * p[8] - p[5] * p[6]);
    seq_t term3 = p[2] * (p[3] * p[7] - p[4] * p[6]);

    return term1 - term2 + term3;
}
