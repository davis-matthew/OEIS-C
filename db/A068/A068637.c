/*
 * A068637  a(n) = Max(n, R(n)), where R(n) (A004086) = digit reversal of n.
 * https://oeis.org/A068637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, hear, look
 * Formula:   a(n) = max(n, rev(n)) where rev(n) is the decimal digit reversal
 *            of n
 * Range:     u8 n <= 102, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast the index to seq_t for computation */
    seq_t x = (seq_t)n;
    seq_t rev = 0;
    seq_t t = x;

    /* Reverse the decimal digits of x */
    while (t != 0)
    {
        seq_t digit = t % 10;   /* get least significant digit */
        rev = rev * 10 + digit; /* build the reversed number */
        t = t / 10;             /* drop the processed digit */
    }

    /* return the larger of the original number and its reversal */
    return (x > rev ? x : rev);
}
