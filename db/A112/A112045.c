/*
 * A112045  Positions of primes (A000040) among nonsquares A000037.
 * https://oeis.org/A112045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A028391(A000040(n)) = p_n - floor(sqrt(p_n))
 * Range:     u8 n <= 54, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   get_prime, isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS sequence A112045 (positions of primes among nonsquares). */

#include "../utils/get_prime.h"
#include "../utils/isqrt.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;          /* offset starts at 1; values for n<1 are undefined */
    seq_t prime = get_prime(n);     /* nth prime, p_n */
    seq_t sqrtp = isqrt(prime);     /* floor(sqrt(p_n)) */
    return prime - sqrtp;           /* p_n - floor(sqrt(p_n)) */
}
