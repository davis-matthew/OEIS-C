/*
 * A023514  a(n) = sum of exponents in prime-power factorization of prime(n) +
 *          1.
 * https://oeis.org/A023514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001222(A000040(n)+1) = sum of exponents in the
 *            prime‑power factorization of prime(n)+1
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
    if (n == 0) return 0;          /* positions below the offset are irrelevant */
    uint32_t p = get_prime(n);     /* p = prime(n) */
    idx_t m = (idx_t)p + 1;        /* m = p + 1 */
    return bigomega(m);            /* sum of exponents in the factorisation of m */
}
