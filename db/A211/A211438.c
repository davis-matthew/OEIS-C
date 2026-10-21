/*
 * A211438  Number of ordered triples (w,x,y) with all terms in
 *          {-n,...,0,...,n} and w+2x+2y=0.
 * https://oeis.org/A211438
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5+3*(-1)^n+2*(7+3*(-1)^n)*n+14*n^2)/8
 * Range:     u8 n <= 3, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The result is an integer for every integer n >= 0. */
seq_t a(idx_t n) {
    /* Compute using separate formulas for even/odd n to avoid
       negative intermediate values. */
    seq_t n64 = (seq_t)n;
    seq_t n2   = n64 * n64;          /* n^2 */
    seq_t num  = (seq_t)14 * n2;     /* 14*n^2 */

    if ((n & (idx_t)1) == 0) {       /* even n : (-1)^n = 1 */
        num += (seq_t)20 * n64 + (seq_t)8;   /* +20*n +8 */
    } else {                          /* odd n : (-1)^n = -1 */
        num += (seq_t)8 * n64 + (seq_t)2;    /* +8*n +2 */
    }

    return num / (seq_t)8;
}
