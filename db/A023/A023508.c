/*
 * A023508  Sum of exponents in prime-power factorization of n-th prime - 1.
 * https://oeis.org/A023508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = bigomega(p_n - 1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    if (n < 1u) return 0;
    uint32_t p = get_prime(n);
    idx_t val = (idx_t)p - 1u;
    return bigomega(val);
}
