/*
 * A166621  a(n) = 10*n - a(n-1), with n>1, a(1)=5.
 * https://oeis.org/A166621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*n - a(n-1) with a(1)=5. Closed form: a(n) = 5*n + 5 if
 *            n is even, 5*n otherwise
 * Range:     u8 n <= 51, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = (seq_t)5 * (seq_t)n;      /* 5*n */
    if ((n & 1) == 0)                    /* n even */
        res += (seq_t)5;                 /* +5 */
    return res;
}
