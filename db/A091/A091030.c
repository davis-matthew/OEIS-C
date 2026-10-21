/*
 * A091030  Partial sums of powers of 13 (A001022).
 * https://oeis.org/A091030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 13*a(n-1) + 1 , a(1) = 1 (equivalently a(n) = (13^n -
 *            1)/12)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;       /* offset 1 – not expected to be called */

    seq_t result = (seq_t)1;    /* a(1) = 1 */

    for (idx_t i = 2; i <= n; ++i)
        result = (seq_t)13 * result + (seq_t)1;

    return result;
}
