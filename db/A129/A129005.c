/*
 * A129005  a(n) = (n^3 + n^2)*5^n.
 * https://oeis.org/A129005
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3 + n^2)*5^n
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 21, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* for n = 0 the result is 0 */
    if (n == 0)
        return 0;

    /* compute 5^n using binary exponentiation */
    seq_t pow5 = 1;
    seq_t base  = 5;
    idx_t exp   = n;
    while (exp)
    {
        if (exp & 1)
            pow5 *= base;
        base *= base;
        exp >>= 1;
    }

    /* compute n^2 and n^3 in seq_t domain */
    seq_t m   = (seq_t)n;
    seq_t n2  = m * m;      /* n^2 */
    seq_t n3  = n2 * m;     /* n^3 */

    /* final value: (n^3 + n^2) * 5^n */
    return (n3 + n2) * pow5;
}
