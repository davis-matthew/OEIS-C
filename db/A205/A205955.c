/*
 * A205955  a(n) = prime(n) * (prime(n+2) - prime(n+1)).
 * https://oeis.org/A205955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n) * (prime(n+2) - prime(n+1))
 * Range:     u8 n <= 13, u16 n <= 427, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;        /* defined only for n ≥ 1 (offset 1) */

    uint32_t pn  = get_prime(n);
    uint32_t pn1 = get_prime(n + 1);
    uint32_t pn2 = get_prime(n + 2);

    seq_t p  = (seq_t)pn;
    seq_t diff = (seq_t)(pn2 - pn1);
    return p * diff;
}
