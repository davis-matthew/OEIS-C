/*
 * A131528  a(n) = (n^1 + 1!)*(n^2 + 2!)*(n^3 + 3!)*(n^4 + 4!)/2!.
 * https://oeis.org/A131528
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(n^2+2)*(n^3+6)*(n^4+24)/2
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 9, u64 n <= 90, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t n4 = n3 * nn;

    seq_t t1 = nn + (seq_t)1;          /* n + 1          */
    seq_t t2 = n2 + (seq_t)2;          /* n^2 + 2        */
    seq_t t3 = n3 + (seq_t)6;          /* n^3 + 6        */
    seq_t t4 = n4 + (seq_t)24;         /* n^4 + 24       */

    /* Divide one (any) even factor by 2 to keep intermediate values small. */
    if ((t1 & (seq_t)1) == 0)
        t1 /= (seq_t)2;
    else if ((t3 & (seq_t)1) == 0)
        t3 /= (seq_t)2;
    else if ((t2 & (seq_t)1) == 0)
        t2 /= (seq_t)2;
    else
        t4 /= (seq_t)2;                /* should not happen but keeps the code safe */

    seq_t result = t1;
    result *= t2;
    result *= t3;
    result *= t4;

    return result;
}
