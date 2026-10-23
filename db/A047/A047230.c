/*
 * A047230  Numbers that are congruent to {3, 4} mod 6.
 * https://oeis.org/A047230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*((n+1)/2)-3 if n is odd, otherwise a(n) = 6*(n/2)-2.
 *            Equivalent form: a(n) = 3*n - 1 - (-1)^n
 * Range:     u8 n <= 85, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;            /* index converted to seq_t */
    seq_t k;                        /* half of the index (rounded up for odd) */
    if (nn & 1)                     /* n is odd */
    {
        k = (nn + 1) / 2;           /* (n+1)/2 as seq_t */
        return 6 * k - 3;           /* 6*k - 3 */
    }
    else                            /* n is even */
    {
        k = nn / 2;                 /* n/2 as seq_t */
        return 6 * k - 2;           /* 6*k - 2 */
    }
}
