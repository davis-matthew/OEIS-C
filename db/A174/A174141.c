/*
 * A174141  Numbers congruent to k mod 25, where 0 <= k <= 9.
 * https://oeis.org/A174141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ((n-1)/10)*25 + ((n-1)%10)
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(1) corresponds to the first term in the OEIS listing */
    if (n == 0) return (seq_t)0;  /* offset handling, not used in tests */

    /* compute n-1 in the same integer type as the output */
    seq_t nn = (seq_t)n;
    seq_t n1 = nn - 1;

    /* integer division and remainder give the required quotient and
       residue modulo 10 */
    seq_t q = n1 / 10;
    seq_t r = n1 % 10;

    /* assemble the final value */
    return q * 25 + r;
}
