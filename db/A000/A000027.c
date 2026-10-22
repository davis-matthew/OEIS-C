/*
 * A000027  The positive integers. Also called the natural numbers, the whole
 *          numbers or the counting numbers, but these terms are ambiguous.
 * https://oeis.org/A000027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  core, nonn, easy, mult, tabl
 * Formula:   a(n) = n
 * Range:     u8 n <= 255, u16 n <= 9895, u32 n <= 9895, u64 n <= 9895,
 *            u128 n <= 9895, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)n;
}
