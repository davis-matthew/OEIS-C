/*
 * A087445  Numbers that are congruent to 1 or 5 mod 12.
 * https://oeis.org/A087445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6*(n-1)-(-1)^n
 * Range:     u8 n <= 43, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* If n is 0 (below offset) we return 0; otherwise compute the
       value using only integer arithmetic. */
    if (n == 0)
        return (seq_t)0;

    seq_t val = (seq_t)6 * ((seq_t)n - 1);  /* 6 * (n-1) */

    if (n & 1)          /* n is odd  -> (-1)^n = -1  => add 1 */
        val += (seq_t)1;
    else                /* n is even -> (-1)^n = +1  => subtract 1 */
        val -= (seq_t)1;

    return val;
}
