/*
 * A073845  a(1)=a(2)=1, a(n+2)=a(n+1)+a(n)+(-2)^n.
 * https://oeis.org/A073845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n+2) = a(n+1) + a(n) + (-2)^n , a(1) = a(2) = 1
 * Range:     i8 n <= 9, i16 n <= 17, i32 n <= 33, i64 n <= 35, i128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {                /* undefined for n < 1, but return 0 safely */
        return (seq_t)0;
    }
    if (n == 1 || n == 2) {
        return (seq_t)1;
    }

    /* a(1) = 1, a(2) = 1 */
    seq_t prev2 = (seq_t)1;      /* a(n-2) in the loop */
    seq_t prev1 = (seq_t)1;      /* a(n-1) in the loop */

    /* (-2)^1 for computation of a(3) */
    seq_t power_neg2 = (seq_t)-2;

    idx_t i;
    for (i = 3; i <= n; ++i) {
        seq_t cur = prev1 + prev2 + power_neg2;
        prev2 = prev1;
        prev1 = cur;
        power_neg2 *= (seq_t)-2;
    }
    return prev1;
}
