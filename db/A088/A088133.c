/*
 * A088133  Sum of first and last digits of n. Different from A115299.
 * https://oeis.org/A088133
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = first digit of n + last digit of n
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sum of first and last decimal digits of the index n. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t last = (seq_t)(n % 10);
    seq_t first = n;
    while (first >= 10) first /= 10;
    return first + last;
}
