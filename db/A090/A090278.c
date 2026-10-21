/*
 * A090278  "Plain Bob Minimus" in bell-ringing is a sequence of permutations
 *          p_1=(1,2,3,4), p_2=(2,1,4,3), .. which runs through all
 *          permutations of {1,2,3,4} with period 24; sequence gives number in
 *          position 2 of n-th permutation.
 * https://oeis.org/A090278
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

/* Period 24. a(n) = a((n-1) % 24 + 1) */
seq_t a(idx_t n)
{
    /* 24‐term period */
    static const seq_t tbl[24] = {
        2, 1, 4, 2, 3, 4, 1, 3, 3, 1,
        2, 3, 4, 2, 1, 4, 4, 1, 3, 4,
        2, 3, 1, 2
    };
    return tbl[(n - 1) % 24];
}
