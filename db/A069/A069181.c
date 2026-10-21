/*
 * A069181  Decimal expansion of 1/1024.
 * https://oeis.org/A069181
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  cons, nonn, easy
 * Formula:   a(n) = floor(10^(n+1) / 1024) mod 10 (the nth decimal digit of
 *            1/1024)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t digits[10] = {0, 0, 0, 9, 7, 6, 5, 6, 2, 5};
    if (n < 10) return digits[n];
    return 0;
}
