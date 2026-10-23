/*
 * A133530  Sum of third powers of three consecutive primes.
 * https://oeis.org/A133530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   _prime_limit_for_idx, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A133530: Sum of third powers of three consecutive primes.
   a(n) = p_n^3 + p_{n+1}^3 + p_{n+2}^3  for n ≥ 1,
   where p_k is the k‑th prime (p_1 = 2).  This implementation builds a
   prime table lazily using the supplied utility functions. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes_upto.h"

/* Static storage for the prime table.  prime_tbl[0] == 2, prime_tbl[1] == 3, ... */
static size_t prime_cnt = 0;          /* number of primes stored */
static uint64_t *prime_tbl = NULL;    /* array of primes */

/* Ensure that prime_tbl contains at least (n+2) primes. */
static void ensure_primes(idx_t n)
{
    /* We must have at least n + 2 primes to compute a(n). */
    if (prime_cnt >= n + 2)
        return;

    /* Determine an upper bound for the (n+2)-th prime. */
    idx_t limit = _prime_limit_for_idx(n + 2);

    /* Build the prime list up to the limit.  build_primes_upto allocates
     * and sets prime_tbl and prime_cnt. */
    build_primes_upto(limit, &prime_tbl, &prime_cnt);
}

/* Compute the nth term of the sequence. */
seq_t
a(idx_t n)
{
    if (n < 1)                /* sequence undefined for n < 1 */
        return (seq_t)0;

    ensure_primes(n);

    /* Fetch the three consecutive primes.  Casting to size_t is safe because
     * n ≥ 1 and prime_cnt ≥ n+2. */
    uint64_t p0 = prime_tbl[(size_t)(n - 1)];
    uint64_t p1 = prime_tbl[(size_t)(n)];
    uint64_t p2 = prime_tbl[(size_t)(n + 1)];

    /* Compute p0^3 + p1^3 + p2^3 in seq_t.  (seq_t) cast ensures all
     * arithmetic is performed in the target type. */
    seq_t term0 = (seq_t)p0 * p0 * p0;
    seq_t term1 = (seq_t)p1 * p1 * p1;
    seq_t term2 = (seq_t)p2 * p2 * p2;
    return term0 + term1 + term2;
}
