/*
 * A122751  Number of essentially different semi-magic squares of order 3 with
 *          semimagic sum n.
 * https://oeis.org/A122751
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            (2*n*(n-1)*(n^2-n+1)-7*(2*n-1)*(-1)^n-8*(-1)^((2*n-1+(-1)^n)/4)+1)/128,
 *            for n ≥ 3
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 221, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < (idx_t)3) return (seq_t)0;        /* offset 3 */

    seq_t N  = (seq_t)n;
    seq_t N2 = N * N;                         /* n^2 */
    seq_t inner = N2 - N + (seq_t)1;          /* n^2 - n + 1 */

    seq_t term = N - (seq_t)1;                /* n-1 */
    term *= N;                                /* (n-1)*n */
    term *= inner;                           /* (n-1)*n*(n^2-n+1) */
    term *= (seq_t)2;                        /* 2*n*(n-1)*(n^2-n+1) */

    /* 7*(2n-1)*(-1)^n : add for odd n, subtract for even n */
    seq_t subterm = (seq_t)2 * N - (seq_t)1;  /* 2n-1 */
    subterm *= (seq_t)7;                     /* 7*(2n-1) */
    if ((n & (idx_t)1) == 0) {               /* n even */
        term -= subterm;
    } else {                                 /* n odd */
        term += subterm;
    }

    term += (seq_t)1;                        /* +1 */

    /* -8*(-1)^((2n-1+(-1)^n)/4) : subtract 8 if n%4<2, else add 8 */
    idx_t mod4 = n & ((idx_t)3);
    if (mod4 < (idx_t)2) {
        term -= (seq_t)8;
    } else {
        term += (seq_t)8;
    }

    return term / (seq_t)128;
}
