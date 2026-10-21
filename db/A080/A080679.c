/*
 * A080679  Lexicographically earliest de Bruijn cycle of length 16 (repeated
 *          indefinitely).
 * https://oeis.org/A080679
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = bit_{15-(n mod 16)} of 0x09AF (the De Bruijn period
 *            0000100110101111)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 0x09AF = 0000 1001 1010 1111 binary */
    static const uint16_t period = 0x09AF;
    uint16_t bit = (period >> (15 - (n & 15))) & 1U;
    return (seq_t)bit;
}
