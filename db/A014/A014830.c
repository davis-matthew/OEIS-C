/*
 * A014830  a(1)=1; for n > 1, a(n) = 7*a(n-1) + n.
 * https://oeis.org/A014830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1; for n>1, a(n)=7*a(n-1)+n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* offset starts at 1; undefined for 0, return 0 safe */
    seq_t result = 1;      /* a(1) */
    if (n == 1)
        return result;

    for (idx_t i = 2; i <= n; ++i)
        result = result * 7 + (seq_t)i;
    return result;
}
