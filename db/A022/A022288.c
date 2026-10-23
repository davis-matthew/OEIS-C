/*
 * A022288  a(n) = n*(31*n-1)/2.
 * https://oeis.org/A022288
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(31*n-1)/2
 * Range:     u8 n <= 4, u16 n <= 65, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t nn = (seq_t)n;
    /* 31*n-1 is always even when n is odd, and n is even when the
       other part is even, allowing safe division first to avoid
       intermediate overflow in small seq_t types. */
    seq_t term = (seq_t)31 * nn - (seq_t)1;
    if (nn & 1)
        return nn * (term / (seq_t)2);
    else
        return (nn / (seq_t)2) * term;
}
