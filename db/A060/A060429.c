/*
 * A060429  a(n) = 4*prime(n)^2+1.
 * https://oeis.org/A060429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*prime(n)^2 + 1
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS sequence A060429 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* obtain the nth prime as a 32‑bit unsigned integer */
    uint32_t p = get_prime(n);

    /* perform the calculation in seq_t to avoid overflow in the
       intermediate steps when seq_t is larger than 32 bits.  */
    seq_t sp  = (seq_t)p;
    seq_t sq  = sp * sp;      /* p^2   */
    seq_t val = sq * 4 + 1;   /* 4*p^2 + 1 */

    return val;
}
