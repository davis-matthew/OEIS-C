/*
 * A165479  a(n) = least i in range [A165478(n),A165478(n+1)] for which
 *          abs(A165477(i)) gets the maximum value in that range.
 * https://oeis.org/A165479
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n/2)*131071 + (1+2*(n mod 2))*43690
 * Range:     u8 none, u16 n <= 0, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* floor(n/2) and n mod 2 */
    seq_t half = (seq_t)(n >> 1);
    seq_t rem  = (seq_t)(n & 1u);

    /* multiplier 1 + 2*rem = 1 when rem=0, 3 when rem=1 */
    seq_t mult = (seq_t)(1 + (rem << 1));

    /* compute components using seq_t arithmetic */
    seq_t part1 = half * (seq_t)131071;          /* floor(n/2) * 131071 */
    seq_t part2 = mult * (seq_t)43690;          /* (1+2*rem) * 43690 */

    return part1 + part2;
}
