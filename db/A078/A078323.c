/*
 * A078323  Arithmetic derivative of n*rad(n)+1, where rad = A007947
 *          (squarefree kernel).
 * https://oeis.org/A078323
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = arithmetic_derivative( n * rad(n) + 1 )
 * Range:     u8 n <= 20, u16 n <= 292, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Factor n to get its distinct primes (rad) and exponents.  */
    uint64_t n64 = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt  = 0;

    factor_u64(n64, primes, exps, &cnt);

    /* rad(n) = product of distinct primes dividing n.  */
    uint64_t rad = 1;
    for (size_t i = 0; i < cnt; ++i)
        rad *= primes[i];

    /* M = n * rad(n) + 1.  */
    uint64_t m = n64 * rad + 1;

    /* Factor M (it always fits in uint64_t for the tested range).  */
    factor_u64(m, primes, exps, &cnt);

    /* Arithmetic derivative: d(m) = Σ e_i * m / p_i.  */
    uint64_t der = 0;
    for (size_t i = 0; i < cnt; ++i)
        der += exps[i] * (m / primes[i]);

    return (seq_t)der;
}
