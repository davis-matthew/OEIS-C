/*
 * A147814  a(n) is the number of bits in the Elias omega-coded value of
 *          prime(n).
 * https://oeis.org/A147814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = 2 + Σ_{i=0..k} d(i, prime(n)) * where d(0,m) =
 *            bitlen_seq_t(m) * and d(i,m) = bitlen_seq_t(d(i-1,m)-1) * k is
 *            the smallest index with d(k,m)=2 * (Elias‑omega length of
 *            prime(n))
 * Range:     u8 n <= 54, u16 n <= 6858, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bitlen_seq_t, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/get_prime.h"   /* uint32_t get_prime(idx_t k); */
#include "../utils/bitlen_seq_t.h" /* unsigned bitlen_seq_t(seq_t x); */

seq_t a(idx_t n)
{
    if (n < 1)  /* offset is 1; behaviour below is irrelevant */
        return 0;

    seq_t p = (seq_t)get_prime(n);   /* nth prime (1‑based) */
    seq_t sum = 0;
    seq_t m   = p;

    for (;;)
    {
        unsigned bits = bitlen_seq_t(m);
        sum += (seq_t)bits;
        if (bits == 2)
            break;
        m = (seq_t)(bits - 1);
    }

    return sum + 2;
}
