/*
 * A023588  a(n) = sum of exponents in prime-power factorization of
 *          2*prime(n)-1.
 * https://oeis.org/A023588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A001222(A076274(n)) = bigomega(2*prime(n)-1)
 * Range:     u8 n <= 31, u16 n <= 4952, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sum of exponents in the prime-power factorization of (2*prime(n)-1). */
#include "../utils/get_prime.h"
#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for offset 0, return 0 */
    uint64_t p = get_prime(n);            /* nth prime (1-indexed) */
    seq_t val = (seq_t)2 * (seq_t)p - 1;   /* 2*prime(n)-1 */
    return bigomega((idx_t)val);          /* number of prime factors counting multiplicity */
}
