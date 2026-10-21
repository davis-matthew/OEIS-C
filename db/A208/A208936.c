/*
 * A208936  Prime production length of the polynomial P = x^2 + x + prime(n):
 *          max { k>0 | P(x) is prime for all x=0,...,k-1 }.
 * https://oeis.org/A208936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 6995, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _prime_limit_for_idx, build_primes_upto, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = the length of prime production of P(x)=x^2+x+prime(n)
   by brute‑force search.  Uses a sieve to obtain primes up to the n‑th
   prime and a generic primality test. */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    /* Cache of the first N primes for reuse across calls. */
    static uint64_t *prime_list = NULL;
    static size_t      prime_cnt  = 0;
    static idx_t       max_n_cached = 0;   /* largest n for which the cache is valid */

    /* Rebuild cache if we need more primes. */
    if (prime_list == NULL || n > max_n_cached) {
        idx_t limit = _prime_limit_for_idx(n);
        free(prime_list);                     /* discard old array */
        build_primes_upto(limit, &prime_list, &prime_cnt);
        max_n_cached = n;
    }

    /* The prime used in the polynomial is the n‑th prime (1‑based). */
    seq_t p = (seq_t)prime_list[n - 1];

    /* Count consecutive primes of P(x)=x^2+x+p. */
    seq_t count = 0;
    for (idx_t x = 0; ; ++x) {
        seq_t val = ((seq_t)x * x) + x + p;
        if (!isPrimeGeneric(val))
            break;
        ++count;
    }
    return count;
}
