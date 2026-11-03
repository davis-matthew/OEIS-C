/*
 * A047937  Number of 2-colorings of an n X n grid, up to rotational symmetry.
 * https://oeis.org/A047937
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (2^(n^2) + 2*2^((n^2 + 3*(n mod 2))/4) + 2^((n^2 + (n mod
 *            2))/2))/4
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Validated: standard, pass, 30 terms through n = 48 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    /* Handle the first two terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    /* Compute the required exponents. */
    idx_t n2     = n * n;                  /* n^2 */
    idx_t nmod2  = n & 1ULL;               /* n mod 2 */
    idx_t exp1   = n2;                     /* exponent for the first term (n^2)    */
    idx_t exp2   = (n2 + 3 * nmod2) / 4;   /* exponent for the middle term (with 2^)*/
    idx_t exp3   = (n2 + nmod2) / 2;       /* exponent for the last term (n^2+1)/2*/

    /* For small seq_t sizes (≤ 64 bits) compute with 128‑bit intermediates. */
    if (sizeof(seq_t) <= 8) {
        __uint128_t t1 = ((__uint128_t)1 << (exp1 - 2));   /* 2^(n^2-2) */
        __uint128_t t2 = ((__uint128_t)1 << (exp2 - 1));   /* 2^((n^2+3*n_mod2)/4-1) */
        __uint128_t t3 = ((__uint128_t)1 << (exp3 - 2));   /* 2^((n^2+n_mod2)/2-2) */

        __uint128_t sum = t1 + t2 + t3;
        return (seq_t)sum;
    }

    /* For larger seq_t types use the arbitrary‑precision big_pow routine. */
    seq_t t1 = ipow((seq_t)2, (seq_t)(exp1 - 2));
    seq_t t2 = ipow((seq_t)2, (seq_t)(exp2 - 1));
    seq_t t3 = ipow((seq_t)2, (seq_t)(exp3 - 2));

    return t1 + t2 + t3;
}
