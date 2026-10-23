/*
 * A014686  In sequence of prime numbers add 1 to first prime, 3rd prime,
 *          fifth prime, ... then subtract 1 from 2nd prime, fourth prime,
 *          sixth prime and so on.
 * https://oeis.org/A014686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n)+(-1)^{n+1} (add 1 for odd n, subtract 1 for even
 *            n)
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the helper get_prime from the utilities */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n<1, safe guard */
    seq_t p = (seq_t)get_prime(n);       /* p is the n-th prime */
    if (n & 1U)                          /* odd n  -> add 1 */
        return p + (seq_t)1;
    else                                 /* even n -> subtract 1 */
        return p - (seq_t)1;
}
