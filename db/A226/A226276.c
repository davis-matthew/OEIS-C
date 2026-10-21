/*
 * A226276  Period 4: repeat [8, 4, 4, 4].
 * https://oeis.org/A226276
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8 if n mod 4 == 0, else 4; period 4 pattern [8,4,4,4]
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use bit masking for speed; n is unsigned */
    if ((n & 3) == 0) {
        return (seq_t)8;
    } else {
        return (seq_t)4;
    }
}
