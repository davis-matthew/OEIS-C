/*
 * A076655  a(n) = numerator(b(n)) where b(1)=1, b(2)=2 and for n > 2, b(n) =
 *          1/b(n-2) - 1/b(n-1).
 * https://oeis.org/A076655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = numerator of b(n) Recurrence: b(1)=1, b(2)=2,
 *            b(n)=1/b(n-2)-1/b(n-1) (n>2) We keep b(n)=p(n)/q(n) with q(n)>0
 *            and reduced
 * Range:     i8 n <= 6, i16 n <= 8, i32 n <= 9, i64 n <= 11, i128 n <= 12,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Requires an existing gcd(seq_t,seq_t) implementation. */
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    /* p_prev2/q_prev2 = b(n-2), p_prev1/q_prev1 = b(n-1) */
    seq_t p_prev2 = (seq_t)1;   /* b(1) */
    seq_t q_prev2 = (seq_t)1;
    seq_t p_prev1 = (seq_t)2;   /* b(2) */
    seq_t q_prev1 = (seq_t)1;

    for (idx_t i = 3; i <= n; ++i)
    {
        /* numerator and denominator before reduction */
        seq_t N = q_prev2 * p_prev1 - q_prev1 * p_prev2;
        seq_t D = p_prev2 * p_prev1;

        /* gcd for reduction, use absolute values */
        seq_t g = gcd(N >= (seq_t)0 ? N : -N, D >= (seq_t)0 ? D : -D);

        seq_t p_cur = N / g;
        seq_t q_cur = D / g;

        /* ensure denominator positive */
        if (q_cur < (seq_t)0) {
            q_cur = -q_cur;
            p_cur = -p_cur;
        }

        /* shift for next iteration */
        p_prev2 = p_prev1;
        q_prev2 = q_prev1;
        p_prev1 = p_cur;
        q_prev1 = q_cur;
    }
    return p_prev1;
}
