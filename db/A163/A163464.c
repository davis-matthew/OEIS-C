/*
 * A163464  Cumulative sum of a repeated shift-and-add operation on the base-7
 *          representation of prime(n).
 * https://oeis.org/A163464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = sum_{k>=1} floor(prime(n)/7^k)
 * Range:     u8 n <= 243, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the cumulative sum of successive right‑shifts of the
   base‑7 representation of the nth prime. */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Get the nth prime.  get_prime returns a 32‑bit value. */
    idx_t p   = (idx_t)get_prime(n);

    /* Accumulate sum of floor(p/7^k) for k>=1. */
    idx_t sum = 0;
    idx_t t   = p / 7;
    while (t > 0) {
        sum += t;
        t   /= 7;
    }

    return (seq_t)sum;
}
