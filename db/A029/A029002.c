/*
 * A029002  Expansion of 1/((1-x)*(1-x^2)*(1-x^3)*(1-x^8)).
 * https://oeis.org/A029002
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + floor((2*n^3 + 42*n^2 + (255+9*(-1)^n)*n)/576)
 * Range:     u8 n <= 1, u16 n <= 25, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert n to seq_t for arithmetic */
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;          /* n^2   */
    seq_t n3 = n2 * nn;          /* n^3   */

    seq_t term1 = 2 * n3;        /* 2*n^3 */
    seq_t term2 = 42 * n2;       /* 42*n^2 */

    /* Compute coefficient 255 + 9*(-1)^n */
    int sign = (n & 1) ? -1 : 1; /* (-1)^n */
    seq_t coeff = (seq_t)(255 + 9 * sign);   /* 255+9*(-1)^n */

    seq_t term3 = coeff * nn;    /* (255+9*(-1)^n)*n */

    seq_t numerator = term1 + term2 + term3;
    seq_t div = numerator / 576; /* floor division */

    return 1 + div;
}
