/*
 * A021193  Decimal expansion of 1/189.
 * https://oeis.org/A021193
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = digit n of the decimal expansion of 1/189 (repeating
 *            block 0,0,5,2,9,1)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The period of 1/189 is 6 digits: 005291. */
    static const seq_t digits[6] = {0, 0, 5, 2, 9, 1};
    return digits[n % 6];
}
