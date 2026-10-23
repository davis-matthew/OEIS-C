/*
 * A128624  Row sums of A128623.
 * https://oeis.org/A128624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(2*n^2 + 4*n + 1 - (-1)^n)/8 Equivalent closed form: if
 *            n is even: a(n) = n^2*(n+2)/4 if n is odd: a(n) = n*((n+1)/2)^2
 * Range:     u8 n <= 9, u16 n <= 63, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* offset starts at 1 */

    seq_t sn = (seq_t)n;
    seq_t result;

    if (n & 1) {                            /* odd n */
        seq_t m = (sn + 1) / 2;              /* (n+1)/2 integer */
        result = sn * m * m;                 /* n * ((n+1)/2)^2 */
    } else {                                /* even n */
        seq_t t = sn / 2;                   /* n/2 integer */
        result = t * t * (sn + 2);          /* (n/2)^2 * (n+2) */
    }
    return result;
}
