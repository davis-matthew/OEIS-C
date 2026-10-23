/*
 * A176593  List of pairs n,13*n.
 * https://oeis.org/A176593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) = (14*n + 12*n*(-1)^n + 1 + (-1)^(n-1)) / 4 Equivalent
 *            simplified form: if n is odd -> a(n) = (n + 1) / 2 if n is even
 *            -> a(n) = 13 * n / 2 (valid for all n >= 1)
 * Range:     u8 n <= 39, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t to perform arithmetic in the result type */
    seq_t nn = (seq_t)n;
    /* Check parity of n using the idx_t type */
    if ((n & (idx_t)1) != 0)           /* odd n */
    {
        return (nn + (seq_t)1) / (seq_t)2;   /* (n + 1) / 2 */
    }
    else                                /* even n */
    {
        return ((seq_t)13 * nn) / (seq_t)2;  /* 13 * n / 2 */
    }
}
