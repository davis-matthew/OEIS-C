/*
 * A080736  Multiplicative function defined by a(1)=1, a(2)=0, a(2^r) =
 *          phi(2^r) (r>1), a(p^r) = phi(p^r) (p odd prime, r>=1), where phi
 *          is Euler's function A000010.
 * https://oeis.org/A080736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = 0 if n mod 4 = 2 a(n) = φ(n) otherwise (Euler's totient
 *            function)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Sequence A080736, multiplicative with a(2)=0, a(2^r)=phi(2^r) (r>1),
   a(p^r)=phi(p^r) for odd prime p. */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    /* All numbers congruent to 2 modulo 4 give 0 */
    if ((n & 3ULL) == 2)
        return (seq_t)0;

    seq_t phi = (seq_t)n;
    idx_t tmp = n;

    /* Factor out the power of 2 */
    if ((tmp & 1ULL) == 0) {
        phi = phi / (seq_t)2 * (seq_t)1;   /* φ(2^k)=2^(k-1) */
        while ((tmp & 1ULL) == 0)
            tmp >>= 1;                     /* divide by 2 */
    }

    /* Factor odd primes */
    for (idx_t p = 3; p <= tmp / p; p += 2) {
        if (tmp % p == 0) {
            phi = phi / (seq_t)p * (seq_t)(p - 1);
            while (tmp % p == 0)
                tmp /= p;
        }
    }

    /* Remaining prime factor (if any) */
    if (tmp > 1)
        phi = phi / (seq_t)tmp * (seq_t)(tmp - 1);

    return phi;
}
