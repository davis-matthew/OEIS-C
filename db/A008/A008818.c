/*
 * A008818  Expansion of (1+2*x^3+x^4)/((1-x^2)^2*(1-x^4)); Molien series for
 *          3-dimensional representation of group 2x = [ 2+,4+ ] = CC_4 = C4.
 * https://oeis.org/A008818
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n*floor(n/2) + 18*floor(n/2) + 8*floor((n+1)/4) - n^2
 *            - 7*n + 8) / 8
 * Range:     u8 n <= 13, u16 n <= 275, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Derived from the closed form listed in the OEIS entry. */

seq_t a(idx_t n)
{
    /* Cast the index to the result type. */
    seq_t nn = (seq_t)n;

    /* floor(n/2) and floor((n+1)/4). */
    seq_t f = nn >> 1;            /* floor(n/2)  */
    seq_t g = (nn + 1) >> 2;      /* floor((n+1)/4) */

    /* Compute the numerator. */
    seq_t num = (seq_t)4 * nn * f;  /* 4*n*floor(n/2)   */
    num += (seq_t)18 * f;          /* +18*floor(n/2)   */
    num += (seq_t)8  * g;          /* +8*floor((n+1)/4)*/
    num -= nn * nn;                /* -n^2             */
    num -= (seq_t)7 * nn;          /* -7*n             */
    num += (seq_t)8;               /* +8               */

    /* Exact division by 8. */
    return num / (seq_t)8;
}
