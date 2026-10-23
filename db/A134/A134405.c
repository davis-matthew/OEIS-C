/*
 * A134405  -1 before list of quadruples -2n-1, 2n+2, -2n, 2n+1.
 * https://oeis.org/A134405
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (1 + (2*n-1)*(-1)^n - 4*(-1)^{floor(n/2)}) / 4
 * Range:     i8 n <= 61, i16 n <= 61, i32 n <= 61, i64 n <= 61, i128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* convert index to seq_t for all arithmetic */
    seq_t n_t = (seq_t)n;
    seq_t two_n = n_t + n_t;
    seq_t two_n_minus1 = two_n - (seq_t)1;

    /* (-1)^n  : +1 when n even, -1 when n odd */
    seq_t sign1 = ((n & 1ULL) == 0ULL) ? (seq_t)1 : (seq_t)-1;

    /* (-1)^{floor(n/2)}  : +1 when floor(n/2) even, -1 when odd */
    idx_t half = n >> 1;
    seq_t sign2 = ((half & 1ULL) == 0ULL) ? (seq_t)1 : (seq_t)-1;

    /* compute the numerator: 1 + (2*n-1)*sign1 - 4*sign2 */
    seq_t term = (seq_t)1 + two_n_minus1 * sign1 - ((seq_t)4) * sign2;

    /* divide by 4, division is exact */
    return term / (seq_t)4;
}
