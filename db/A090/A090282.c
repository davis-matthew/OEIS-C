/*
 * A090282  "Plain Bob Minimus" in bell-ringing is a sequence of permutations
 *          p_1=(1,2,3,4), p_2=(2,1,4,3), .. which runs through all
 *          permutations of {1,2,3,4} with period 24; sequence gives position
 *          of bell 2 in n-th permutation.
 * https://oeis.org/A090282
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

/* A090282 – Plain Bob Minimus: position of bell 2 in the nth permutation.
   The sequence is periodic with period 24.  The following table gives the
   first 24 values; for all n ≥ 1 we return a((n-1) mod 24).  This satisfies
   the recurrence given in the OEIS entry. */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* 24‑term cycle of the sequence */
    static const seq_t period[24] = {
        2, 1, 1, 2, 3, 4, 4, 3, 4, 3, 2, 1,
        1, 2, 3, 4, 3, 4, 4, 3, 2, 1, 1, 2
    };

    /* n is 1‑based; use modulo 24 to index into the period */
    size_t i = (size_t)((n - 1) % 24);
    return period[i];
}
