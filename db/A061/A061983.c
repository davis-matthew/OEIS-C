/*
 * A061983  a(n) = 3^n - (3*n^2 + n + 2)/2.
 * https://oeis.org/A061983
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n - (3*n^2 + n + 2)/2
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 3^n by exponentiation by squaring */
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)3;
    idx_t e = n;
    while (e > 0)
    {
        if (e & 1)
            result *= base;
        base *= base;
        e >>= 1;
    }

    seq_t nn   = (seq_t)n;
    seq_t temp = nn * nn;      /* n^2 */
    temp *= (seq_t)3;          /* 3*n^2 */
    temp += nn;                /* + n */
    temp += (seq_t)2;          /* + 2 */
    seq_t sub = temp / (seq_t)2;

    return result - sub;
}
