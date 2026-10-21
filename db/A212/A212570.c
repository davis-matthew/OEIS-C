/*
 * A212570  Number of (w,x,y,z) with all terms in {1,...,n} and
 *          |w-x|=|x-y|+|y-z|.
 * https://oeis.org/A212570
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(-1-3*(-1)^n+10*n^2)/12
 * Range:     u8 n <= 3, u16 n <= 23, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Reduced: if n even   a(n) = (5*n^3 - 2*n)/6 */
/*          if n odd    a(n) = (5*n^3 + n)/6 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* cast to seq_t */
    seq_t n2 = nn * nn;           /* n^2   */
    seq_t n3 = n2 * nn;           /* n^3   */
    seq_t num;

    if ((n & 1) == 0) {           /* n is even */
        num = 5 * n3 - 2 * nn;    /* (5*n^3 - 2*n) */
    } else {                      /* n is odd */
        num = 5 * n3 + nn;        /* (5*n^3 + n)   */
    }

    return num / 6;               /* exact division */
}
