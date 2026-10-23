/*
 * A177154  Fractional part of the conversion from degrees Celsius to
 *          Fahrenheit.
 * https://oeis.org/A177154
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10 - 2*n) mod 10
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t table[5] = {
        (seq_t)0,
        (seq_t)8,
        (seq_t)6,
        (seq_t)4,
        (seq_t)2
    };
    return table[n % 5];
}
