/*
 * A204454  Odd numbers not divisible by 11.
 * https://oeis.org/A204454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n - 1 + 2*floor((n+4)/10) (n >= 1)
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 2*n - 1 */
    seq_t result = (seq_t)2 * n;
    result -= 1;

    /* Compute 2 * floor((n+4)/10) */
    idx_t div = (n + 4) / 10;              /* n >= 1 => n+4 >= 5 > 0 so truncation = floor  */
    result += (seq_t)2 * (seq_t)div;

    return result;
}
