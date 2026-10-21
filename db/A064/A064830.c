/*
 * A064830  a(n) = gcd(n, prime(n)^2 - 1).
 * https://oeis.org/A064830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(n, prime(n)^2 - 1)
 * Range:     u8 n <= 263, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/get_prime.h"

/* Compute the n‑th term of sequence A064830.
   n is 1‑based; a(0) is undefined and returns 0. */
seq_t a(idx_t n)
{
    if (n == 0)   /* safety for off‑by‑one callers */
        return 0;

    /* Obtain the n‑th prime.  get_prime returns a uint32_t. */
    uint32_t p32 = get_prime(n);
    uint64_t p   = (uint64_t)p32;
    uint64_t nn  = (uint64_t)n;

    /* Compute (p^2 – 1) modulo n.  Using modular reduction avoids
       overflow even when p^2 is large. */
    uint64_t p_mod = p % nn;
    uint64_t sq_mod = (p_mod * p_mod) % nn;
    uint64_t val_mod = (sq_mod + nn - 1) % nn;   /* (p^2 – 1) mod n */

    /* The gcd is taken in seq_t.  The result never exceeds n,
       so it fits into seq_t for the tested ranges. */
    return gcd((seq_t)n, (seq_t)val_mod);
}
