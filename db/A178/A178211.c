/*
 * A178211  a(n) = prime(n)^2 mod n.
 * https://oeis.org/A178211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n)^2 mod n (offset 1)
 * Range:     u8 n <= 54, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   get_prime, mod_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/mod_mul.h"

/* Computes the nth term of OEIS A178211:
   a(n) = (prime(n))^2 mod n ,  with n>=1.
   Works for seq_t of any supported type (fixed‑width or arbitrary precision). */
seq_t a(idx_t n)
{
    if (n == 0)    /* safety for unexpected calls */
        return 0;

    /* 1‑based index: get the nth prime */
    uint32_t p = get_prime(n);
    seq_t prime = (seq_t)p;
    seq_t modn  = (seq_t)n;

    /* r = prime mod n  (fits into 64 bits because 0 <= r < n <= 2^64-1) */
    seq_t r = prime % modn;
    uint64_t ru = (uint64_t)r;            /* convert to native integer */

    /* Compute (r*r) mod n using 128‑bit modular multiplication.
       For any seq_t, r and n fit into 64 bits, so mod_mul is safe. */
    uint64_t mu = mod_mul(ru, ru, (uint64_t)n);

    return (seq_t)mu;                     /* cast back to seq_t */
}
