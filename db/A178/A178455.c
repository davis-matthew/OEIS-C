/*
 * A178455  Partial sums of floor(2^n/7).
 * https://oeis.org/A178455
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(2^(n+1)/7) - floor((n+1)/3)
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 128,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* part2 = floor((n+1)/3) */
    seq_t part2 = ((seq_t)(n + 1)) / 3;

    seq_t part1;
    /* For fixed‑width types we use an untaken 128‑bit intermediate
     * to avoid overflow of 2^(n+1).  For arbitrary‑precision types
     * pow2(n+1) builds a correct big integer. */
    if (sizeof(seq_t) <= 8) {
        __uint128_t num = ((__uint128_t)1) << (n + 1);
        part1 = (seq_t)(num / 7);
    } else {
        seq_t pw = pow2(n + 1);
        part1 = pw / (seq_t)7;
    }

    return part1 - part2;
}
