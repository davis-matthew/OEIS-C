/*
 * A141101  a(n) = prime(2*n) - 2*n.
 * https://oeis.org/A141101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(2*n) - 2*n
 * Range:     u8 n <= 33, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* 2*n as idx_t; shift left by one to avoid overflow of signed types */
    idx_t k = n << 1;                  /* k = 2*n */
    seq_t p = (seq_t)get_prime(k);     /* (2n)-th prime */
    return p - (seq_t)k;               /* prime(2n) - 2*n */
}
