/*
 * A117329  Determinants of 3 X 3 matrices of discrete blocks of 9 consecutive
 *          primes.
 * https://oeis.org/A117329
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = determinant of the 3x3 matrix formed from the n‑th block
 *            of 9 consecutive primes (indices 1‑based)
 * Range:     i8 n <= 1, i16 n <= 15, i32 n <= 51, i64 n <= 51, i128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Global cache of primes */
static uint32_t *prime_arr = NULL;   /* array of generated primes      */
static size_t prime_count = 0;        /* number of primes stored        */

/* Ensure that prime_arr contains at least the first 'req_idx' primes
   (1‑based prime index).  This expands the table if necessary, using a
   simple sieve.  The calculation uses idx_t arithmetic; intermediate
   variables are cast to size_t where appropriate to avoid overflow in
   C++.
*/
static void ensure_primes(idx_t n)
{
    /* Number of primes required: the last prime of block n is the
       (n * 9)-th prime. */
    size_t req_idx = (size_t)(n * 9);

    if (prime_count >= req_idx)
        return; /* already have enough primes */

    /* Estimate an upper bound for the req_idx-th prime.
       For x >= 6, p_x <= x (log x + log log x).  We use a loose
       bound 15 * x + 100 which is more than enough for the given
       limits. */
    idx_t limit = (idx_t)(req_idx * 15 + 100);
    if (limit < 30)
        limit = 30;                      /* safety for tiny n */

    /* Sieve of Eratosthenes on a byte array */
    unsigned char *is_prime = (unsigned char *)malloc((size_t)limit + 1);
    if (!is_prime)
        return; /* allocation failed – cannot compute the sequence */
    memset(is_prime, 1, (size_t)limit + 1);
    is_prime[0] = is_prime[1] = 0;
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;
        }
    }

    /* Allocate / expand the primes array */
    uint32_t *new_arr = (uint32_t *)realloc(prime_arr, req_idx * sizeof(uint32_t));
    if (!new_arr) {
        free(is_prime);
        return; /* allocation failed */
    }
    prime_arr = new_arr;
    prime_count = 0;
    for (idx_t i = 2; i <= limit && prime_count < req_idx; ++i) {
        if (is_prime[i])
            prime_arr[prime_count++] = (uint32_t)i;
    }

    free(is_prime);
}

/* Main sequence function.  Computes the determinant using the 9
   primes of the n‑th block. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* undefined for n=0, return 0 */

    /* Ensure primes up to the required block. */
    ensure_primes(n);

    /* Starting index (0‑based) of the 9 primes for block n. */
    size_t start = (size_t)((n - 1) * 9);

    /* Retrieve the 9 primes. */
    seq_t p1 = (seq_t)prime_arr[start + 0];
    seq_t p2 = (seq_t)prime_arr[start + 1];
    seq_t p3 = (seq_t)prime_arr[start + 2];
    seq_t p4 = (seq_t)prime_arr[start + 3];
    seq_t p5 = (seq_t)prime_arr[start + 4];
    seq_t p6 = (seq_t)prime_arr[start + 5];
    seq_t p7 = (seq_t)prime_arr[start + 6];
    seq_t p8 = (seq_t)prime_arr[start + 7];
    seq_t p9 = (seq_t)prime_arr[start + 8];

    /* Determinant of the 3×3 matrix:
          | p1 p2 p3 |
          | p4 p5 p6 |
          | p7 p8 p9 |
       D = p1*p5*p9 + p2*p6*p7 + p3*p4*p8
           - p1*p6*p8 - p2*p4*p9 - p3*p5*p7
    */
    seq_t det = p1 * p5 * p9
              + p2 * p6 * p7
              + p3 * p4 * p8
              - p1 * p6 * p8
              - p2 * p4 * p9
              - p3 * p5 * p7;
    return det;
}
