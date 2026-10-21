/*
 * A053833  Sum of digits of n written in base 13.
 * https://oeis.org/A053833
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = sum of digits of n written in base 13 Recurrence: a(0) =
 *            0, a(13*n + i) = a(n) + i (0 <= i <= 12)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (; n; n /= 13)
        sum += (seq_t)(n % 13);
    return sum;
}
