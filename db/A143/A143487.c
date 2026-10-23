/*
 * A143487  "Fourth down, Extream [sic] between the two farthest Bells from
 *          it" in bell-ringing is a sequence of permutations p_1=(1,2,3,4),
 *          p_2=(1,2,4,3), .. which runs through all permutations of {1,2,3,4}
 *          with period 24; sequence gives number in position 4 of n-th
 *          permutation.
 * https://oeis.org/A143487
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) repeats every 24 terms: a(1..24) =
 *            4,3,3,3,2,2,2,4,4,2,2,2,1,1,1,4,4,1,1,1,3,3,3,4
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 24-term periodic pattern */
    static const seq_t pattern[24] = {
        4, 3, 3, 3,
        2, 2, 2, 4,
        4, 2, 2, 2,
        1, 1, 1, 4,
        4, 1, 1, 1,
        3, 3, 3, 4
    };
    /* n >= 1, shift to 0-based index */
    idx_t idx = (n - 1) % 24;
    return pattern[idx];
}
