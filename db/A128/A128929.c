/*
 * A128929  Diameter of a special type of regular graph of degree 4 whose
 *          order maintain an increase in form of an arithmetic progression.
 * https://oeis.org/A128929
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n+2)/4) for n >= 4
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t v = (seq_t)n + 2;
    return v / 4;
}
