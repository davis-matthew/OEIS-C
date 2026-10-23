/*
 * A168333  a(n) = (14*n + 7*(-1)^n + 1)/4.
 * https://oeis.org/A168333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = (14*n + 7*(-1)^n + 1)/4
 * Range:     u8 n <= 36, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n must be >= 1; behavior for n < 1 is unspecified */
    if (n < 1)
        return 0;

    seq_t nn   = (seq_t)n;
    seq_t prod = (seq_t)7 * nn;    /* 7*n */

    /* If n is even: a = (7*n + 4)/2
       If n is odd : a = (7*n - 3)/2   */
    if ((n & 1) == 0)             /* even n */
        return (prod + (seq_t)4) / (seq_t)2;
    else                          /* odd n */
        return (prod - (seq_t)3) / (seq_t)2;
}
