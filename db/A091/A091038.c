/*
 * A091038  Second column (k=6) of array A090216 ((5,5)-Stirling2) divided by
 *          5*5! = 600.
 * https://oeis.org/A091038
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (120^(n-2)) * (6^(n-1)-1) / 5 , n >= 2
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp)
    {
        if (exp & (idx_t)1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;                /* undefined for n < 2 – return 0 */

    /* compute 120^(n-2) and 6^(n-1) */
    seq_t pow120 = pow_seq_t((seq_t)120, n - 2);
    seq_t pow6   = pow_seq_t((seq_t)6,   n - 1);

    /* (6^(n-1) - 1) / 5 is guaranteed to be integer */
    seq_t part = (pow6 - (seq_t)1) / (seq_t)5;

    return pow120 * part;
}
