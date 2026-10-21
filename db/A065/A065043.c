/*
 * A065043  Characteristic function of the numbers with an even number of
 *          prime factors (counted with multiplicity): a(n) = 1 if n =
 *          A028260(k) for some k then 1 else 0.
 * https://oeis.org/A065043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 - Ω(n) mod 2 where Ω(n) is the total number of prime
 *            factors of n (counted with multiplicity). Thus a(n) = 1 if n has
 *            an even number of prime factors, else 0
 * Range:     u8 n <= 9567, u16 n <= 9567, u32 n <= 9567, u64 n <= 9567,
 *            u128 n <= 9567, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Handle the first term (n == 1): Ω(1) = 0, even -> a(1) = 1 */
    if (n <= 1) return 1;

    idx_t rem = n;           /* remaining part of n to factor */
    seq_t parity = 0;        /* 0 = even number of factors seen so far,
                               1 = odd */

    /* factor out 2 separately */
    while ((rem & 1) == 0) {
        rem >>= 1;
        parity ^= 1;          /* toggle parity for each factor of 2 */
    }

    /* trial division by odd primes up to sqrt(rem) */
    idx_t p = 3;
    for (; p * p <= rem; p += 2) {
        while (rem % p == 0) {
            rem /= p;
            parity ^= 1;      /* toggle parity for each factor of p */
        }
    }

    /* if something greater than 1 remains, it is a prime factor itself */
    if (rem > 1) {
        parity ^= 1;
    }

    /* parity == 0 => even number of factors, return 1; else return 0 */
    return parity ? 0 : 1;
}
