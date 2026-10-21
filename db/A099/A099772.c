/*
 * A099772  2^(p^2) where p is prime.
 * https://oeis.org/A099772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(p_n^2)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   get_prime, pow_seq_large
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/pow_seq_large.h"

seq_t a(idx_t n)
{
    uint32_t p = get_prime(n);
    seq_t exp = (seq_t)p * (seq_t)p;
    return pow_seq_large((seq_t)2, exp);
}
