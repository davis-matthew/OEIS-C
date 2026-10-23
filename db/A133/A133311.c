/*
 * A133311  The nonnegative integers reordered in groups of 5 even numbers
 *          followed by 5 odd numbers.
 * https://oeis.org/A133311
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = g * 10 + 2 * r - 9 * floor(r / 5) where g = floor(n /
 *            10), r = n mod 10
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t g = n / 10;
    idx_t r = n % 10;
    seq_t res = (seq_t)g * 10 + (seq_t)2 * (seq_t)r - (seq_t)9 * (seq_t)(r / 5);
    return res;
}
