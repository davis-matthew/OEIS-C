/*
 * A080929  Sequence associated with a(n) = 2*a(n-1) + k*(k+2)*a(n-2).
 * https://oeis.org/A080929
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(n+2)*2^(n-2) (for n>0, with a(0)=1)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 24, u64 n <= 54, u128 n <= 116,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)3;
    seq_t term = (seq_t)(n + 1) * (seq_t)(n + 2);
    term <<= (n - 2);            /* multiply by 2^(n-2) */
    return term;
}
