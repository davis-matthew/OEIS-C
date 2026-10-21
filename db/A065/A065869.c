/*
 * A065869  a(n) = (2^(prime(n)^2) + 1)/(2^prime(n) + 1).
 * https://oeis.org/A065869
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2^(prime(n)^2) + 1) / (2^prime(n) + 1)
 * Range:     u8 none, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   big_pow, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th term of OEIS sequence A065869.   */
/* A helper function get_prime(idx_t) returns the n‑th prime. */
/* big_pow(seq_t base, seq_t exp) computes base^exp.     */

#include "../utils/get_prime.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* no defined value for n < 2 */

    /* prime(p) fits in an int for the required n */
    int p = get_prime(n);            /* p = prime(n) */
    seq_t pp = (seq_t)p;

    /* exponent: p^2 */
    seq_t exp = pp * pp;

    /* numerator: 2^(p^2) + 1 */
    seq_t num = big_pow((seq_t)2, exp) + 1;

    /* denominator: 2^p + 1 */
    seq_t den = big_pow((seq_t)2, pp) + 1;

    return num / den;
}
