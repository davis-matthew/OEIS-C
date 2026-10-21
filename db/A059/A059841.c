/*
 * A059841  Period 2: Repeat [1,0]. a(n) = 1 - (n mod 2); Characteristic
 *          function of even numbers.
 * https://oeis.org/A059841
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 - (n mod 2)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return 1 - (seq_t)(n & 1);
}
