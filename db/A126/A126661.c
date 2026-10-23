/*
 * A126661  Array read by antidiagonals: A(i,j) = smallest odd prime r such
 *          that pqr+2 is prime, where p is the i-th odd prime and q is the
 *          j-th odd prime.
 * https://oeis.org/A126661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, tabl, nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A126661: a(n) = smallest odd prime r such that oddprime(k)*oddprime(l)*r+2 is prime
   where (k,l) are the antidiagonal coordinates of n.  Compute on demand. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"   /* get_prime(idx_t k) returns the k‑th prime (1‑based) */
#include "../utils/is_prime_big.h" /* is_prime_big(seq_t n) tests primality */

/* Determine the antidiagonal coordinates (i,j) for a given 1‑based offset n. */
static void antidiag_coords(idx_t n, idx_t *i, idx_t *j)
{
    /* Find smallest d with d*(d+1)/2 >= n */
    idx_t d = 1;
    while (d * (d + 1ULL) / 2ULL < n)
        ++d;

    idx_t prev = d * (d - 1ULL) / 2ULL;          /* terms before this diagonal */
    idx_t offset = n - prev;                    /* 1‑based offset on the diagonal */

    *i = offset;                /* 1‑based index for oddprime(i) */
    *j = d - offset + 1ULL;     /* 1‑based index for oddprime(j) */
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* undefined for n < 1 */

    idx_t i, j;
    antidiag_coords(n, &i, &j);

    /* oddprime(k) = get_prime(k+1) because get_prime(1)=2, we need the odd primes */
    seq_t pi = get_prime(i + 1ULL); /* oddprime(i) */
    seq_t pj = get_prime(j + 1ULL); /* oddprime(j) */

    /* Search for the smallest odd prime r (starting from 3) with pi*pj*r+2 prime */
    for (idx_t idx = 0; ; ++idx) {
        seq_t r = get_prime(idx + 2ULL);           /* r = 3,5,7,... */
        seq_t val = pi * pj * r + 2ULL;
        if (is_prime_big(val))
            return r;
    }

    /* Unreachable */
    return 0;
}
