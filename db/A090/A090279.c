/*
 * A090279  "Plain Bob Minimus" in bell-ringing is a sequence of permutations
 *          p_1=(1,2,3,4), p_2=(2,1,4,3), .. which runs through all
 *          permutations of {1,2,3,4} with period 24; sequence gives number in
 *          position 3 of n-th permutation.
 * https://oeis.org/A090279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Periodic sequence of period 24; values are the number in the third
   position of the n‑th Plain Bob Minimus permutation. */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* n is 1‑based; protect against underflow */

    static const seq_t period[24] = {
        3, 4, 1, 3, 2, 1, 4, 2,
        4, 2, 1, 4, 3, 1, 2, 3,
        2, 3, 1, 2, 4, 1, 3, 4
    };

    idx_t idx = (n - 1) % 24;      /* 0‑based index within the period */
    return period[(size_t)idx];
}
