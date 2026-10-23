/*
 * A103876  a(n) = -1/10 (mod prime(n)): A test for divisibility by the n-th
 *          prime.
 * https://oeis.org/A103876
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = p - (10 mod p)^(-1) where p = prime(n)
 * Range:     u8 n <= 54, u16 n <= 6756, u32 n <= 9614, u64 n <= 9614,
 *            u128 n <= 9614, big any n
 * Helpers:   get_prime, modinv
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/modinv.h"


seq_t a(idx_t n)
{
    /* 1. Obtain the n‑th prime (OEIS indexing starts at 1). */
    seq_t p = get_prime(n);

    /* 2. Compute the modular inverse of 10 modulo p. */
    /*    The helper modinv takes idx_t arguments and returns seq_t. */
    seq_t inv = modinv((idx_t)10, (idx_t)p);

    /* 3. The desired term is p - inv (always non‑negative). */
    return p - inv;
}
