/*
 * A022815  Number of terms in 5th derivative of a function composed with
 *          itself n times.
 * https://oeis.org/A022815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n^2+13*n-2)/24
 * Range:     u8 n <= 6, u16 n <= 32, u32 n <= 563, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset start at 1, but guard against 0 */

    seq_t nn = (seq_t)n;

    seq_t a1 = nn;                                     /* n          */
    seq_t b1 = nn + 1;                                  /* n + 1      */
    seq_t c1 = nn * nn + (seq_t)13 * nn - 2;            /* n^2 + 13n – 2 */

    /* Divide the product a1*b1*c1 by 24 in a way that keeps intermediate
       values small. 24 = 2*2*2*3, so we successively divide by 2, 2, 2, 3. */
    const seq_t factors[4] = {2, 2, 2, 3};
    for (int i = 0; i < 4; ++i) {
        seq_t f = factors[i];
        if (a1 % f == 0) {
            a1 /= f;
        } else if (b1 % f == 0) {
            b1 /= f;
        } else {
            /* c1 must be divisible by f since the whole product is divisible by 24 */
            c1 /= f;
        }
    }

    seq_t temp = a1 * b1;
    return temp * c1;
}
