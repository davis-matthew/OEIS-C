/*
 * A159219  Number of n X n arrays of squares of integers with every 2X2
 *          subblock summing to 18.
 * https://oeis.org/A159219
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + D(n) * where a(2)=30, a(3)=78 * D(3)=18 *
 *            D(n)=26-16*2^{floor((n-4)/2)} for n>=4 * This recursive formula
 *            reproduces OEIS A159219
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                /* undefined for n < 2, return 0 */
    if (n == 2)
        return (seq_t)30;        /* a(2) */

    seq_t a_prev = 30;           /* current a(i-1) */
    seq_t pow2   = 1;            /* 2^{floor((i-4)/2)} for current i (i>=4) */

    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t d;
        if (i == 3)
            d = 18;               /* D(3) */
        else
        {
            /* When i is even and >=6 a new pair for floor((i-4)/2) starts */
            if ((i & 1ULL) == 0 && i >= 6)
                pow2 *= 2;        /* pow2 = 2 * old pow2 */
            d = 26 - ((seq_t)16 * pow2); /* D(i) */
        }

        a_prev = a_prev * 2 + d;     /* a(i) = 2*a(i-1) + D(i) */
    }

    return a_prev; /* a(n) */
}
