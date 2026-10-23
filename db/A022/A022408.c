/*
 * A022408  a(n) = a(n-1) + a(n-2) + 1, with a(0)=3, a(1)=9.
 * https://oeis.org/A022408
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + 1 Initial values: a(0) = 3, a(1) = 9
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first two terms directly. */
    if (n == (idx_t)0) return (seq_t)3;
    if (n == (idx_t)1) return (seq_t)9;

    /* Iteratively compute a(n) using only a few variables. */
    seq_t a_minus2 = (seq_t)3;   /* a(0) */
    seq_t a_minus1 = (seq_t)9;   /* a(1) */
    seq_t current;

    for (idx_t i = (idx_t)2; i <= n; ++i)
    {
        current   = a_minus1 + a_minus2 + (seq_t)1;
        a_minus2  = a_minus1;
        a_minus1  = current;
    }

    return a_minus1;
}
