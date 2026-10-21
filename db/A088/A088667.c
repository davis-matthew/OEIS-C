/*
 * A088667  n^4 + 6 mod 10.
 * https://oeis.org/A088667
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n^4 + 6) mod 10
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Since the sequence is periodic with period 10, we can use a lookup table. */
    static const seq_t table[10] = {6, 7, 2, 7, 2, 1, 2, 7, 2, 7};
    return table[n % 10];
}
