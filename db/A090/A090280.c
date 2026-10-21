/*
 * A090280  "Plain Bob Minimus" in bell-ringing is a sequence of permutations
 *          p_1=(1,2,3,4), p_2=(2,1,4,3), .. which runs through all
 *          permutations of {1,2,3,4} with period 24; sequence gives number in
 *          position 4 of n-th permutation.
 * https://oeis.org/A090280
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) has period 24, so a(n) = period[(n-1) mod 24]. Initial
 *            values: 4, 3, 3, 1, 1, 2, 2, 4, 2, 4, 4, 1, 1, 3, 3, 2, 3, 2, 2,
 *            1, 1, 4, 4, 3
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Static table of the first 24 values.  All values fit in
       seq_t (max 4). */
    static const seq_t period[24] = {
        /* n = 1..24 */
        4, 3, 3, 1, 1, 2, 2, 4,
        2, 4, 4, 1, 1, 3, 3, 2,
        3, 2, 2, 1, 1, 4, 4, 3
    };

    /* For n ≥ 1 compute the index inside one period. */
    idx_t pos = (n - 1) % 24;   /* pos is 0..23 */
    return period[pos];
}
