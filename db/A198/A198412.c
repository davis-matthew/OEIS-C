/*
 * A198412  a(n) = (3^(6*n) - 2^(6*n))/35.
 * https://oeis.org/A198412
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = (3^(6*n) - 2^(6*n))/35
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* exp = 6*n (as idx_t for the pow functions) */
    idx_t exp_idx = n * (idx_t)6;

    /* For small seq_t types we compute using a 128‑bit intermediate
       to avoid overflow for all n whose result fits in seq_t. */
    if (sizeof(seq_t) <= 8) {
        __uint128_t exp = ( __uint128_t)exp_idx;   /* exp ≤ 60000 */

        __uint128_t pow3 = 1;
        for (uint64_t i = 0; i < (uint64_t)exp; ++i) pow3 *= 3;

        __uint128_t pow2 = 1;
        for (uint64_t i = 0; i < (uint64_t)exp; ++i) pow2 *= 2;

        __uint128_t diff = pow3 - pow2;
        return (seq_t)(diff / 35);
    }

    /* For arbitrary‑precision seq_t we use seq_pow which works with
       the seq_t type. */
    seq_t pow3 = seq_pow((seq_t)3, exp_idx);
    seq_t pow2 = seq_pow((seq_t)2, exp_idx);
    seq_t diff = pow3 - pow2;
    return diff / (seq_t)35;
}
