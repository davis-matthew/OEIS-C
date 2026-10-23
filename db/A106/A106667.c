/*
 * A106667  a(n) = 1 if prime(n) + 2 is a prime, a(n) = -1 if prime(n) + 2 is
 *          a semiprime, otherwise 0.
 * https://oeis.org/A106667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign, less
 * Formula:   a(n) = 1 if prime(n)+2 is prime, * -1 if prime(n)+2 is
 *            semiprime, * 0 otherwise * (OEIS A106667)
 * Range:     i8 n <= 30, i16 n <= 3529, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   get_prime, is_prime_uint64, is_semiprime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"        /* provides seq_t get_prime(idx_t) */
#include "../utils/is_prime_uint64.h"  /* provides bool is_prime_uint64(uint64_t) */
#include "../utils/is_semiprime_uint64.h" /* provides bool is_semiprime_uint64(uint64_t) */

seq_t a(idx_t n)
{
    /* get the n‑th prime (indices start at 1) */
    seq_t p = get_prime(n);

    /* candidate to test: prime(n)+2 */
    seq_t cand = p + (seq_t)2;

    /* test for primality */
    if (is_prime_uint64((uint64_t)cand))
        return (seq_t)1;

    /* test for semiprimality */
    if (is_semiprime_uint64((uint64_t)cand))
        return (seq_t)(-1);

    /* otherwise 0 */
    return (seq_t)0;
}
