/*
 * A091553  Third column (k=6) sequence of array A090214 ((4,4)-Stirling2)
 *          divided by 72.
 * https://oeis.org/A091553
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (15*360^n - 10*120^n + 24^n) / 6
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_big(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp > 0)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (exp)  /* avoid one unnecessary multiplication at the end */
            base *= base;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t pow360 = pow_big((seq_t)360, n);
    seq_t pow120 = pow_big((seq_t)120, n);
    seq_t pow24  = pow_big((seq_t)24,  n);

    seq_t numerator = 15 * pow360 - 10 * pow120 + pow24;
    return numerator / 6;
}
