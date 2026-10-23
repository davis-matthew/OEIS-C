/*
 * A023581  Sum of exponents in prime-power factorization of p(n)+3.
 * https://oeis.org/A023581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = bigomega( get_prime(n) + 3 )
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bigomega.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* 0 is outside the OEIS offset */
    uint32_t p  = get_prime(n);            /* n‑th prime (1‑indexed)   */
    idx_t     m = (idx_t)p + (idx_t)3;     /* p(n) + 3                 */
    return bigomega(m);                    /* sum of exponents of m    */
}
