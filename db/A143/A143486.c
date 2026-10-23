/*
 * A143486  "Fourth down, Extream [sic] between the two farthest Bells from
 *          it" in bell-ringing is a sequence of permutations p_1=(1,2,3,4),
 *          p_2=(1,2,4,3), .. which runs through all permutations of {1,2,3,4}
 *          with period 24; sequence gives number in position 3 of n-th
 *          permutation.
 * https://oeis.org/A143486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a_pattern[(n-1) mod 24] where a_pattern contains the
 *            period of the sequence. The values are obtained by simulating
 *            the permutation process described in the OEIS entry
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Periodic sequence – 24 distinct values */
    static const seq_t a_pattern[24] = {
        3, 4, 2, 2, 3, 3, 4, 2, 2, 4, 1, 1,
        2, 2, 4, 1, 1, 4, 3, 3, 1, 1, 4, 3
    };
    /* n is 1‑based; convert to 0‑based index within period */
    idx_t idx = (n - 1) % 24;
    return a_pattern[idx];
}
