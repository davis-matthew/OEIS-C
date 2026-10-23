/*
 * A138673  a(n) = prime(n)^4 mod prime(n-1).
 * https://oeis.org/A138673
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n)^4 mod prime(n-1), offset 2 (a(2) is the first
 *            term)
 * Range:     u8 n <= 54, u16 n <= 6547, u32 n <= 10000, u64 n <= 10000,
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
    if (n < 2)  /* offset is 2; behavior below is irrelevant */
        return 0;

    /* Retrieve the nth and (n-1)th primes. get_prime returns a 32‑bit value, cast to seq_t. */
    seq_t pn  = (seq_t)get_prime(n);       /* prime(n)    */
    seq_t pn1 = (seq_t)get_prime(n - 1);   /* prime(n-1)  */

    /* Compute prime(n)^4 mod prime(n-1) using the provided modular exponentiation. */
    return mod_pow(pn, 4ULL, pn1);
}
