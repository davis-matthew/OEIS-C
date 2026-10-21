/*
 * A217254  a(n) = round(primepi(n) * prime(n)/n).
 * https://oeis.org/A217254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less, easy
 * Formula:   a(n) = round( primepi(n) * prime(n) / n )
 * Range:     u8 n <= 13, u16 n <= 281, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses the built‑in utilities prime_pi() and
 * get_prime() to obtain the prime counting function π(n) and the nth
 * prime p(n).  Rounding of the rational number π(n)∙p(n)/n is
 * performed by adding ⌊n⁄2⌋ before integer division, which implements
 * standard round-to-nearest with ties away from zero for positive
 * operands.  All arithmetic is done in the target type seq_t, which
 * may be 8, 16, 32, 64 or 128 bits, or an arbitrary–precision type,
 * so the code works for every build configuration. */

#include "../utils/prime_pi.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Protect against accidental misuse for n<1 – the sequence is
     * defined only for positive indices.  The caller commits to
     * providing a valid n, so simply return 0 for n==0. */
    if (n == 0)
        return 0;

    /* π(n) – number of primes ≤ n.  The function returns a seq_t. */
    seq_t pi = prime_pi(n);

    /* p(n) – the nth prime.  The utility returns a 32‑bit unsigned
     * integer; cast to seq_t so all following arithmetic uses the
     * same type. */
    seq_t p = (seq_t)get_prime(n);

    /* Convert n to seq_t for the arithmetic below. */
    seq_t nn = (seq_t)n;

    /* Number of primes × nth prime. */
    seq_t numer = pi * p;

    /* Half of n, truncated as integer.  Adding this before division
     * implements round-to-nearest for positive values. */
    seq_t half = nn / 2;

    /* Final rounded quotient. */
    return (numer + half) / nn;
}
