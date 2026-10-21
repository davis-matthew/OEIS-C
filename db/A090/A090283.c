/*
 * A090283  "Plain Bob Minimus" in bell-ringing is a sequence of permutations
 *          p_1=(1,2,3,4), p_2=(2,1,4,3), .. which runs through all
 *          permutations of {1,2,3,4} with period 24; sequence gives position
 *          of bell 3 in n-th permutation.
 * https://oeis.org/A090283
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

/* Sequence A090283: Periodic with 24 values.
   a(1)=3, a(2)=4, a(3)=4, a(4)=3, a(5)=2, a(6)=1, a(7)=1, a(8)=2,
   a(9)=2, a(10)=1, a(11)=1, a(12)=2, a(13)=3, a(14)=4, a(15)=4,
   a(16)=3, a(17)=4, a(18)=3, a(19)=2, a(20)=1, a(21)=1, a(22)=2,
   a(23)=3, a(24)=4.  The sequence repeats thereafter.
*/
seq_t a(idx_t n)
{
    static const seq_t period[24] = {
        3, 4, 4, 3, 2, 1, 1, 2,
        2, 1, 1, 2, 3, 4, 4, 3,
        4, 3, 2, 1, 1, 2, 3, 4
    };
    unsigned int idx = (unsigned int)((n - 1) % 24);
    return period[idx];
}
