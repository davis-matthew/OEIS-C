/*
 * A141454  A Legendre symbol type assignment of the modulo ten primes to the
 *          polynomial: Expand[(x-1)*(x+1)*(x-2)*(x+2)*(x-0)]=4 x - 5 x^3 +
 *          x^5; c(n) = If[Mod[n, 10] == 1, 1, If[Mod[n, 10] == 9, -1,
 *          If[Mod[n, 10] == 3, 2, If[Mod[n, 10] == 7, -2, 0]]]] such that n
 *          is a prime[n].
 * https://oeis.org/A141454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  uned, sign
 * Formula:   a(n) is the value assigned to the n-th prime, according to its
 *            residue modulo 10: 1 → 1, 9 → –1, 3 → 2, 7 → –2, otherwise → 0
 * Range:     i8 n <= 100, i16 n <= 100, i32 n <= 100, i64 n <= 100,
 *            i128 n <= 100, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;    /* unused – sequence starts at n = 1 */

    uint32_t p = get_prime(n);

    switch (p % 10) {
        case 1: return (seq_t) 1;
        case 9: return (seq_t)-1;
        case 3: return (seq_t) 2;
        case 7: return (seq_t)-2;
        default: return (seq_t) 0;
    }
}
