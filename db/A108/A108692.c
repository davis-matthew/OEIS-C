/*
 * A108692  It is known that 4472988326827347533 is a quadratic non-residue
 *          for all primes between 3 and 283; sequence gives
 *          4472988326827347533 mod prime(n).
 * https://oeis.org/A108692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4472988326827347533 mod p_n, where p_n is the n-th prime
 * Range:     u8 n <= 54, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include <stdint.h>

static const uint64_t CONST_C = 4472988326827347533ULL;

/* Computes the n-th term of sequence A108692. */
seq_t a(idx_t n)
{
    /* Get the n-th prime (1‑based index). */
    uint64_t prime = (uint64_t)get_prime((uint64_t)n);
    /* Compute the constant modulo that prime. */
    uint64_t rem = CONST_C % prime;
    /* Convert the result back to seq_t. */
    return (seq_t)rem;
}
