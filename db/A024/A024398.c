/*
 * A024398  a(n) = [ (2nd elementary symmetric function of S(n))/(first
 *          elementary symmetric function of S(n)) ], where S(n) = {first n+1
 *          positive integers congruent to 2 mod 3}.
 * https://oeis.org/A024398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/4)*(3*(n^2-n-1) - (-1)^floor(n/2))
 * Range:     u8 n <= 9, u16 n <= 234, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* offset 1: a(1) = 0 */
    if (n <= 1) return (seq_t)0;

    seq_t nseq = (seq_t)n;
    seq_t sub  = nseq * nseq - nseq - (seq_t)1;  /* n^2 - n - 1 */
    seq_t term = (seq_t)3 * sub;                 /* 3*(n^2-n-1) */

    /* compute (-1)^floor(n/2) sign */
    int parity = (int)((n / 2) & 1);  /* 1 if floor(n/2) is odd, else 0 */

    if (parity) {
        /* sign = -1, so subtract(-1) == add 1 */
        term = term + (seq_t)1;
    } else {
        /* sign = +1, so subtract 1 */
        term = term - (seq_t)1;
    }

    /* final result: divide by 4 */
    return term / (seq_t)4;
}
