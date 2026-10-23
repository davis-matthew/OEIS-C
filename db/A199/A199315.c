/*
 * A199315  a(n) = (11*5^n+1)/2.
 * https://oeis.org/A199315
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*5^n+1)/2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow   = 1;           /* 5^0 = 1 */
    seq_t base  = 5;           /* base for exponentiation */
    idx_t exp   = n;           /* working copy of n */

    /* binary exponentiation */
    while (exp != 0)
    {
        if (exp & 1)
            pow = pow * base;
        /* avoid a needless square on the last loop when exp == 1,
           though it does not hurt for small seq_t types */
        if (exp > 1)
            base = base * base;
        exp >>= 1;
    }

    /* compute (11*pow + 1) / 2 */
    return (pow * (seq_t)11 + 1) / 2;
}
