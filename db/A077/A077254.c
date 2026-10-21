/*
 * A077254  a(n) = prime(n)^n mod n.
 * https://oeis.org/A077254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n)^n mod n
 * Range:     u8 n <= 54, u16 n <= 6720, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* offset starts at 1 */
    seq_t prime = (seq_t)get_prime(n);         /* get the nth prime */
    seq_t mod   = (seq_t)n;                    /* modulus n */
    return mod_pow(prime, (uint64_t)n, mod);   /* prime^n mod n */
}
