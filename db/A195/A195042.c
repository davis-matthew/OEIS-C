/*
 * A195042  Concentric 9-gonal numbers.
 * https://oeis.org/A195042
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n^2 + 5/2*((-1)^n - 1))/4 Equivalent integer
 *            expressions: if (n even) a = 9*n^2 / 4 if (n odd) a = (9*n^2 -
 *            5) / 4
 * Range:     u8 n <= 10, u16 n <= 170, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast index to seq_t for arithmetic */
    seq_t nn = (seq_t)n;
    /* n^2 */
    seq_t n2 = nn * nn;
    seq_t res;

    /* check parity of n */
    if ((n & 1) == 0)          /* even n */
        res = (9 * n2) / 4;
    else                      /* odd n */
        res = (9 * n2 - 5) / 4;

    return res;
}
