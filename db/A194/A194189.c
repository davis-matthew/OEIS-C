/*
 * A194189  Number of primes between the n-th triangular number and the n-th
 *          square.
 * https://oeis.org/A194189
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{p | T_n < p < n²} (primes strictly between the n‑th
 *            triangular number T_n = n(n+1)/2 and the n‑th square S_n = n²)
 * Range:     u8 n <= 15, u16 n <= 444, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, first_ge, first_gt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"
#include "../utils/first_ge.h"
#include "../utils/first_gt.h"

/* Static storage for the prime list */
static uint64_t *prime_list   = NULL;   /* array of primes */
static size_t   prime_count  = 0;       /* number of primes stored */
static uint64_t sieve_limit  = 0;       /* largest limit already sieved */

/* Rebuild the prime list for all primes up to `limit`.  The helper
   function provided by the framework allocates a new array and
   assigns it to `prime_list`.  The previous array, if any, is freed. */
static void rebuild_sieve(uint64_t limit)
{
    if (prime_list != NULL) {
        free(prime_list);
        prime_list = NULL;
        prime_count = 0;
    }
    build_primes_upto(limit, &prime_list, &prime_count);
    sieve_limit = limit;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Compute the triangular and square numbers using seq_t arithmetic. */
    seq_t n_seq = (seq_t)n;
    seq_t tri   = n_seq * (n_seq + 1) / 2;   /* T_n */
    seq_t sq    = n_seq * n_seq;              /* n² */

    /* Ensure we have primes up to at least the square number. */
    if ((uint64_t)sq > sieve_limit)
        rebuild_sieve((uint64_t)sq);

    /* Count primes strictly greater than tri and strictly less than sq. */
    size_t idx_gt_tri = first_gt(prime_list, prime_count, (uint64_t)tri);
    size_t idx_ge_sq  = first_ge(prime_list, prime_count, (uint64_t)sq);

    /* The number of primes in (tri, sq) is the difference of indices. */
    return (seq_t)(idx_ge_sq - idx_gt_tri);
}
