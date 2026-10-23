/*
 * A168152  a(n) = prime(prime(n)) - prime(n).
 * https://oeis.org/A168152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(prime(n)) - prime(n)
 * Range:     u8 n <= 18, u16 n <= 919, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* undefined, safe return */
    uint32_t p   = get_prime(n);      /* p_n = nth prime   */
    uint32_t pp  = get_prime((idx_t)p); /* prime(p_n)       */
    seq_t   res  = (seq_t)pp - (seq_t)p;
    return res;
}
