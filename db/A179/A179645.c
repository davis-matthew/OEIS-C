/*
 * A179645  a(n) = prime(n)^8.
 * https://oeis.org/A179645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n)^8
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 54, u128 n <= 1000,
 *            big any n
 * Helpers:   get_prime, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* n >= 1 is assumed */
    seq_t p = (seq_t)get_prime((idx_t)n);
    return seq_pow(p, (idx_t)8);
}
