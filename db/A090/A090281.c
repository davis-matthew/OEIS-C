/*
 * A090281  "Plain Bob Minimus" in bell-ringing is a sequence of permutations
 *          p_1=(1,2,3,4), p_2=(2,1,4,3), ... which runs through all
 *          permutations of {1,2,3,4} with period 24; sequence gives position
 *          of bell 1 (the treble bell) in n-th permutation.
 * https://oeis.org/A090281
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8192, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Periodic pattern with period 8:
   a(n) = { 1, 2, 3, 4, 4, 3, 2, 1 } repeated */
seq_t a(idx_t n)
{
    static const seq_t pattern[8] = {1, 2, 3, 4, 4, 3, 2, 1};
    /* n >= 1; subtract 1 to get 0-based index, then mask for period 8 */
    return pattern[(n - 1) & 7];
}
