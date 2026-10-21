/*
 * A096333  Prime numbers that are 2 less than a prime-indexed odd triangular
 *          number or 1 more than a prime-indexed even triangular number.
 * https://oeis.org/A096333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 41, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric, next_prime, xmalloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A096333 – “Prime numbers that are 2 less than a prime‑indexed odd
    triangular number or 1 more than a prime‑indexed even triangular number.”
    The implementation iterates over successive prime indices, computes the
    corresponding triangular number, applies the required adjustment, tests
    the result for primality, and caches all accepted terms.  The cache
    is preallocated large enough to cover all requested indices up to the
    limits of typical test suites (≈100 000).  No dynamic reallocation is
    performed on the sequence array, avoiding problems with non‑POD
    seq_t types.  */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/xmalloc.h"
#include "../utils/isPrimeGeneric.h"
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    /* static deterministic cache */
    static seq_t *terms   = NULL;      /* array of accepted primes      */
    static size_t terms_len = 0;       /* number of terms already found */
    static const size_t CACHE_CAP = 65536; /* generous static capacity   */

    /* initialise cache once */
    if (terms == NULL) {
        terms = (seq_t *)xmalloc(CACHE_CAP * sizeof(*terms));
    }

    if (n == 0) {                /* OEIS offset 1: undefined for n = 0 */
        return (seq_t)0;
    }

    /* index variable to store the last prime index processed */
    static uint64_t last_prime_index = 1;

    while (terms_len < (size_t)n) {
        /* obtain next prime index */
        last_prime_index = next_prime(last_prime_index);

        /* triangular number for this prime index */
        seq_t p = (seq_t)last_prime_index;
        seq_t T = p * (p + 1) / 2;

        /* adjust according to parity of T */
        seq_t cand = (T & 1) ? T - 2 : T + 1;

        /* keep only the primes */
        if (isPrimeGeneric(cand)) {
            terms[terms_len++] = cand;
        }
    }

    return terms[n - 1];          /* a(1) corresponds to terms[0] */
}
