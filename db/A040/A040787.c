/*
 * A040787  Continued fraction for sqrt(816).
 * https://oeis.org/A040787
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 28, a(n>0) is periodic with period 8: 1,1,3,3,3,1,1,56 */
seq_t a(idx_t n)
{
    /* period 8 pattern for indices n ≥ 1 */
    static const seq_t pattern[8] = {
        (seq_t)1, (seq_t)1, (seq_t)3, (seq_t)3,
        (seq_t)3, (seq_t)1, (seq_t)1, (seq_t)56
    };

    if (n == 0)
        return (seq_t)28;

    /* use bitwise AND since 8 is a power of two */
    idx_t idx = (n - 1) & 7;
    return pattern[idx];
}
