/*
 * A211539  Number of ordered triples (w,x,y) with all terms in {1,...,n} and
 *          2w = 2n - 2x + y.
 * https://oeis.org/A211539
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + (n-1)/2 for odd n, a(n-1) + n for even n
 *            Equivalent closed form: a(n) = 1/16 * (6n^2 + 2n - 3 +
 *            (2n+3)*(-1)^n) Simplified: if n even: a(n) = (6n^2 + 4n)/16 if n
 *            odd : a(n) = (6n^2 - 6)/16 The function implements the
 *            simplified form using only integer arithmetic on seq_t
 * Range:     u8 n <= 6, u16 n <= 163, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;          /* cast index to seq_t */
    seq_t six   = (seq_t)6;
    seq_t four  = (seq_t)4;
    seq_t six_c = (seq_t)6;          /* constant 6 used in odd case */

    seq_t n2 = n_seq * n_seq;        /* n^2 */
    seq_t six_n2 = six * n2;        /* 6*n^2 */

    if ((n & 1) == 0) {             /* even n */
        seq_t four_n = four * n_seq;   /* 4*n */
        seq_t numer  = six_n2 + four_n; /* 6n^2 + 4n */
        return numer >> 4;            /* divide by 16 */
    } else {                         /* odd n */
        seq_t numer = six_n2 - six_c;  /* 6n^2 - 6 */
        return numer >> 4;            /* divide by 16 */
    }
}
