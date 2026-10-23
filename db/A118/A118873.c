/*
 * A118873  Determinant of n-th continuous block of 4 consecutive squares of
 *          primes.
 * https://oeis.org/A118873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = prime(n)^2*prime(n+3)^2 - prime(n+1)^2*prime(n+2)^2
 * Range:     i8 n <= 1, i16 n <= 6, i32 n <= 32, i64 n <= 32, i128 n <= 32,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* obtain the four consecutive primes needed */
    uint64_t p1 = get_prime(n);
    uint64_t p2 = get_prime(n + 1);
    uint64_t p3 = get_prime(n + 2);
    uint64_t p4 = get_prime(n + 3);

    /* compute (p1*p4) and (p2*p3) in 128-bit to avoid overflow */
    __int128 a1 = (__int128)p1 * p4;
    __int128 a2 = (__int128)p2 * p3;

    /* difference and sum of the two products */
    __int128 diff = a1 - a2;
    __int128 sum  = a1 + a2;

    /* final result = diff * sum, still in 128-bit */
    __int128 result = diff * sum;

    /* cast back to seq_t (safe because result fits into seq_t if it
       fits into the declared type, and for arbitrary‑precision builds
       the conversion is handled by the class constructor). */
    return (seq_t)result;
}
