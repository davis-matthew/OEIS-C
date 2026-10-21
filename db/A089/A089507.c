/*
 * A089507  Second column of triangle A089504 and second column of array
 *          A078741 divided by 18.
 * https://oeis.org/A089507
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*24^n - 6^n) / 3
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    /* Compute 6^n and 24^n */
    seq_t pow6  = pow_seq_t((seq_t)6,  n);
    seq_t pow24 = pow_seq_t((seq_t)24, n);

    /* (4*24^n - 6^n) / 3 */
    seq_t numerator = (seq_t)4 * pow24 - pow6;
    return numerator / (seq_t)3;
}
