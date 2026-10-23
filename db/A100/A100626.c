/*
 * A100626  Numbers of the form 2^(2p+1) where p is prime.
 * https://oeis.org/A100626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(2*p_n+1) where p_n is the n‑th prime
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 18,
 *            big any n
 * Helpers:   estimate_prime_upper_bound, seq_pow, simple_sieve
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation generates primes on demand via a simple sieve
   and uses seq_pow to compute powers of 2.  The sieve is expanded
   whenever a larger n is requested.  All intermediate values are
   kept within the natural range of the supplied types. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Utilities provided by the build system */
#include "../utils/simple_sieve.h"
#include "../utils/seq_pow.h"
#include "../utils/estimate_prime_upper_bound.h"

/* Compute the n-th term of A100626.  The function handles calls in
   any order and caches primes up to the largest n requested. */
seq_t a(idx_t n)
{
    /* n is assumed to start from 1 (offset of the sequence) */
    if (n == (idx_t)0)
        return (seq_t)0;  /* undefined by the sequence, return 0 */

    static uint32_t *prime_list = NULL;
    static size_t prime_count  = 0;

    if ((size_t)n > prime_count) {
        /* Need more primes → regenerate the sieve up to an estimate
           that is guaranteed to contain at least n primes. */
        uint64_t est = estimate_prime_upper_bound((uint64_t)n);
        uint32_t limit = (uint32_t)est;  /* safe: estimate < 2^32 for n <= 10^5 */

        uint32_t *new_list = NULL;
        size_t new_count  = 0;
        simple_sieve(limit, &new_list, &new_count);

        /* Replace old storage if present. */
        free(prime_list);
        prime_list  = new_list;
        prime_count = new_count;
    }

    /* Fetch the (n‑1)th element (0‑based indexing). */
    uint32_t prime = prime_list[(size_t)n - 1];
    idx_t exponent = (idx_t)2 * (idx_t)prime + 1;

    /* Compute 2^exponent using seq_pow. */
    seq_t result = seq_pow((seq_t)2, exponent);
    return result;
}
