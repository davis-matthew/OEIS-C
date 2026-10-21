/*
 * A075110  Concatenation of n-th prime and n in decimal notation.
 * https://oeis.org/A075110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A000040(n) * 10^(A004216(n)+1) + n (concatenate the n‑th
 *            prime and the decimal representation of n)
 * Range:     u8 n <= 9, u16 n <= 99, u32 n <= 9999, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"   /* uint32_t get_prime(idx_t n); */

seq_t a(idx_t n)
{
    /* offset is 1; a(0) is undefined – return 0 for safety */
    if (n == 0)
        return (seq_t)0;

    /* 1. obtain the n‑th prime (as a uint32_t), cast to seq_t          */
    seq_t prime = (seq_t)get_prime(n);

    /* 2. count decimal digits of n                                      */
    idx_t digits = 0;
    idx_t tmp    = n;
    do {
        ++digits;
        tmp /= 10;
    } while (tmp);

    /* 3. compute 10^digits as seq_t                                   */
    seq_t pow10 = (seq_t)1;
    idx_t i;
    for (i = 0; i < digits; ++i)
        pow10 *= (seq_t)10;

    /* 4. concatenate: prime * 10^digits + n                            */
    seq_t result = prime * pow10 + (seq_t)n;

    return result;
}
