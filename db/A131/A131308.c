/*
 * A131308  Alternate A001477 and tripled 2*A000027.
 * https://oeis.org/A131308
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n % 4 == 0 ? floor(n/4) : 2 * (floor(n/4) + 1))
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t q = ((seq_t)n) / 4;      /* floor(n/4)          */
    seq_t r = ((seq_t)n) % 4;      /* n mod 4              */
    if (r == 0)
        return q;                 /* even multiples of 4  */
    else
        return 2 * (q + 1);       /* otherwise even numbers */
}
