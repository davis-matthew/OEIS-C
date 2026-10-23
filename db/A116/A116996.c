/*
 * A116996  Partial sums of A116966.
 * https://oeis.org/A116996
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(n+1)/2 if n mod 4 != 2, otherwise a(n) = n*(n+1)/2 + 1
 * Range:     u8 n <= 22, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t val = nn * (nn + 1) / 2;
    if ((n & 3) == 2)  /* n % 4 == 2 */
        val += 1;
    return val;
}
