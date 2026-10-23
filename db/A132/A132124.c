/*
 * A132124  a(n) = n*(n+1)*(8*n + 1)/6.
 * https://oeis.org/A132124
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(8*n + 1)/6
 * Range:     u8 n <= 5, u16 n <= 36, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t operands */
    seq_t t1 = (seq_t)n;            /* n          */
    seq_t t2 = (seq_t)n + 1;        /* n + 1      */
    seq_t t3 = (seq_t)8 * (seq_t)n + 1; /* 8*n + 1 */

    /* One of the three numbers is even; divide that one by 2 */
    if ((t1 & 1) == 0)
        t1 >>= 1;
    else if ((t2 & 1) == 0)
        t2 >>= 1;
    else
        t3 >>= 1;                    /* t3 must be even */

    /* One of the three numbers is divisible by 3; divide that one by 3 */
    if (t1 % 3 == 0)
        t1 /= 3;
    else if (t2 % 3 == 0)
        t2 /= 3;
    else
        t3 /= 3;                    /* t3 must be divisible by 3 */

    /* Final multiplication – all intermediates fit in seq_t */
    return t1 * t2 * t3;
}
