/*
 * A154052  Number of planar triangular n X n X n nonnegative integer grids
 *          symmetric both under 120 degree rotation and reflection with every
 *          similarly oriented 5 X 5 X 5 subtriangle summing to 5.
 * https://oeis.org/A154052
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n ≡ 1 (mod 3) for n ≥ 13, otherwise a(n) is given by
 *            the following exceptional values: a(5)=0, a(6)=5, a(7)=8,
 *            a(8)=0, a(9)=2, a(10)=3, a(11)=0, a(12)=1
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Values for n < 13 (offset 5) */
    static const seq_t small[8] = {
        0,   /* n = 5  */
        5,   /* n = 6  */
        8,   /* n = 7  */
        0,   /* n = 8  */
        2,   /* n = 9  */
        3,   /* n = 10 */
        0,   /* n = 11 */
        1    /* n = 12 */
    };

    if (n < 5)
        return 0;          /* unspecified, return 0 */

    if (n < 13)
        return small[n - 5];

    /* For n ≥ 13: pattern 1,0,0 repeating (n ≡ 1 (mod 3) → 1) */
    return (n % 3 == 1) ? (seq_t)1 : (seq_t)0;
}
