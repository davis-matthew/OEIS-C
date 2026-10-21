/*
 * A226570  a(n) = Sum_{k=1..n} (k+1)! mod n.
 * https://oeis.org/A226570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ( sum_{k=1..n} (k+1)! ) mod n
 * Range:     u8 n <= 267, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mod_mul.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* safety for out-of-range input */
    uint64_t mod  = (uint64_t)n;          /* modulus fits in 64 bits */
    uint64_t sum  = 0;
    uint64_t fact = 1;

    /* compute factorials 2! ... (n+1)! modulo n and accumulate the sum */
    for (uint64_t i = 2; i <= n + 1; ++i) {
        fact = mod_mul(fact, i, mod);            /* factorial mod n */
        __uint128_t tmp = (__uint128_t)sum + fact;/* avoid overflow */
        sum = (uint64_t)(tmp % mod);              /* sum modulo n */
    }
    return (seq_t)sum;
}
