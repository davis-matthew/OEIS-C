/*
 * A143485  "Fourth down, Extream [sic] between the two farthest Bells from
 *          it" in bell-ringing is a sequence of permutations p_1=(1,2,3,4),
 *          p_2=(1,2,4,3), .. which runs through all permutations of {1,2,3,4}
 *          with period 24; sequence gives number in position 2 of n-th
 *          permutation.
 * https://oeis.org/A143485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Periodic sequence with period 24; a(1)=2, a(2)=2, a(3)=4,
 *            a(4)=1, a(5)=1, a(6)=4, a(7)=3, a(8)=3, a(9)=1, a(10)=1,
 *            a(11)=4, a(12)=3, a(13)=3, a(14)=4, a(15)=2, a(16)=2, a(17)=3,
 *            a(18)=3, a(19)=4, a(20)=2, a(21)=2, a(22)=4, a(23)=1, a(24)=1
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Period of length 24 */
    static const seq_t period[24] = {
        2, 2, 4, 1, 1, 4, 3, 3,
        1, 1, 4, 3, 3, 4, 2, 2,
        3, 3, 4, 2, 2, 4, 1, 1
    };

    /* n is 1‑based; compute (n-1) mod 24 safely */
    size_t pos = (size_t)((n - 1) % 24);
    return period[pos];
}
