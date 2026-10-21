/*
 * A232719  Sum_{k=1,...,2n} (-1)^k binomial(8*n,4*k).
 * https://oeis.org/A232719
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-1)^n/2 * ((2+sqrt(2))^(4*n) + (2-sqrt(2))^(4*n)) - 1
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 4, i64 n <= 9, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* t0 = 1, t1 = 68, with recurrence t_k = 136*t_{k-1} - 16*t_{k-2} (k >= 2) */
    seq_t t_prev2 = (seq_t)1;
    seq_t t_prev1 = (seq_t)68;

    /* compute t_n for k = 2 .. n */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t t_curr = (seq_t)136 * t_prev1 - (seq_t)16 * t_prev2;
        t_prev2 = t_prev1;
        t_prev1 = t_curr;
    }

    /* a(n) = (-1)^n * t_n - 1 */
    seq_t res = (n & (idx_t)1) ? -t_prev1 : t_prev1;
    res -= (seq_t)1;
    return res;
}
