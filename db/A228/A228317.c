/*
 * A228317  The hyper-Wiener index of the triangular graph T(n) (n >= 1).
 * https://oeis.org/A228317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n - 1)*(n - 2)*(3*n - 5)/8, a(1)=0, a(2)=0
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;

    seq_t nn = (seq_t)n;

    seq_t v = nn * (nn - (seq_t)1);   /* n*(n-1) */
    v *= (nn - (seq_t)2);            /* n*(n-1)*(n-2) */

    seq_t three_n = nn * (seq_t)3;    /* 3*n  (use seq_t to avoid overflow) */
    seq_t tmp     = three_n - (seq_t)5; /* 3*n - 5 */

    v *= tmp;                        /* multiply by (3*n-5) */
    v /= (seq_t)8;                   /* divide by 8 (fits in seq_t) */

    return v;
}
