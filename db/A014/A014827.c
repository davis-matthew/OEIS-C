/*
 * A014827  a(1)=1, a(n) = 5*a(n-1) + n.
 * https://oeis.org/A014827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) + n, a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(1) is the first term in the sequence */
    if (n <= 1)
        return (seq_t)1;

    seq_t result = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i)
    {
        result = 5 * result + (seq_t)i;
    }
    return result;
}
