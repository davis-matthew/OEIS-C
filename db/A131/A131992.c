/*
 * A131992  a(n) = 1 + prime(n) + prime(n)^2 + prime(n)^3 + prime(n)^4.
 * https://oeis.org/A131992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + prime(n) + prime(n)^2 + prime(n)^3 + prime(n)^4
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 54, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* OEIS offset: a(1) is the first term. */
    if (n == 0) return 0;                 /* undefined for n < 1 – handled gracefully */
    uint32_t pk = get_prime(n);            /* k‑th prime (k ≥ 1) */
    seq_t p   = (seq_t)pk;                 /* cast to seq_t type */
    seq_t res = 1;                         /* start with the leading 1 */
    seq_t term = p;                        /* current power of p */

    for (int i = 0; i < 4; ++i) {
        res += term;                       /* add p^i  (i = 0..3) */
        /* multiply using a 128‑bit intermediate to avoid overflow in uint64_t builds */
        __uint128_t mult = ( __uint128_t )term * ( __uint128_t )p;
        term = (seq_t)mult;                /* promote back to seq_t */
    }
    return res;
}
