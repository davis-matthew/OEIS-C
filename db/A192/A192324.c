/*
 * A192324  Sequence of numbers formed as remainder of Mersenne numbers
 *          divided by primes.
 * https://oeis.org/A192324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2^n - 1) mod prime(n)
 * Range:     u8 n <= 54, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mod_pow.h"
#include "../utils/get_prime.h"

/* Compute the n-th term of OEIS sequence A192324.
   The index n is 1‑based; a(1) = 1, a(2) = 0, …  */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for n = 0 – return 0 defensively */
        return 0;

    /* Obtain the n-th prime (1‑based).  The helper returns a 32‑bit value. */
    uint32_t p = get_prime(n);

    /* Compute 2^n modulo p.  mod_pow returns a value in the same type as the
       modulus, which is safe for all supported seq_t sizes. */
    seq_t powmod = mod_pow((seq_t)2, n, (seq_t)p);

    /* The remainder of (2^n - 1) divided by p.
       Avoid underflow when powmod == 0 (happens only when p == 2). */
    seq_t result;
    if (powmod == (seq_t)0)
        result = (seq_t)p - (seq_t)1;   /* (p - 1) mod p  ==  -1 mod p */
    else
        result = powmod - (seq_t)1;

    return result;
}
