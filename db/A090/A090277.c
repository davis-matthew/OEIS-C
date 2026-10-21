/*
 * A090277  "Plain Bob Minimus" in bell-ringing is a sequence of permutations
 *          p_1=(1,2,3,4), p_2=(2,1,4,3), .. which runs through all
 *          permutations of {1,2,3,4} with period 24; sequence gives number in
 *          position 1 of n-th permutation.
 * https://oeis.org/A090277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) has period 24 – the sequence repeats every 24 terms
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* 24‑term period of the Plain Bob Minimus sequence. */
    static const seq_t period[24] = {
        1, 2, 2, 4, 4, 3, 3, 1,
        1, 3, 3, 2, 2, 4, 4, 1,
        1, 4, 4, 3, 3, 2, 2, 1
    };
    /* n is 1‑based; compute zero‑based index modulo 24. */
    size_t idx = (size_t)((n - 1) % 24);
    return period[idx];
}
