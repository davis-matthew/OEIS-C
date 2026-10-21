/*
 * A075827  Let u(1) = x and u(n+1) = (n^2/u(n)) + 1 for n >= 1; then a(n) is
 *          such that u(n) =(a(n)*x + b(n))/(c(n)*x + d(n)) (in lowest terms)
 *          and a(n), b(n), c(n), d(n) are positive integers.
 * https://oeis.org/A075827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A024167(n)/gcd(A024167(n), A024167(n-1)) where A024167
 *            and A024168 are the coefficients in the continued fraction
 *            representation of u(n). The function computes the recurrence for
 *            the coefficients, reduces each step, and finally returns the
 *            required ratio
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 20, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* The first two terms are defined explicitly. */
    if (n <= 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)1;

    /* We maintain (a_{k-1}, b_{k-1}) and (a_k, b_k) where
       u(k) = (a_k x + b_k)/(a_{k-1} x + b_{k-1})  in lowest terms. */
    seq_t a_prev2 = (seq_t)1;   /* a_1 */
    seq_t b_prev2 = (seq_t)0;   /* b_1 */

    seq_t a_prev  = (seq_t)1;   /* a_2 */
    seq_t b_prev  = (seq_t)1;   /* b_2 */

    /* Compute up to a_n.  For each k from 2 to n-1 we produce
       (a_{k+1}, b_{k+1}) using k as the index in the recurrence. */
    for (idx_t k = 2; k < n; ++k)
    {
        /* k^2 * (a_{k-1}, b_{k-1}) + (a_k, b_k)  →  numerator of u(k+1) */
        seq_t k2   = (seq_t)k * (seq_t)k;
        seq_t anum = k2 * a_prev2 + a_prev;
        seq_t bnum = k2 * b_prev2 + b_prev;

        /* The complete fraction before reduction is
           (anum x + bnum) / (a_prev x + b_prev).
           Cancel any common integer factor that divides all four
           coefficients. */
        seq_t g1 = gcd(anum, bnum);
        seq_t g2 = gcd(a_prev, b_prev);
        seq_t g  = gcd(g1, g2);

        anum /= g;
        bnum /= g;
        a_prev /= g;
        b_prev /= g;

        /* Shift the window: previous becomes behind,
           current becomes the new a_prev. */
        a_prev2 = a_prev;
        b_prev2 = b_prev;
        a_prev  = anum;
        b_prev  = bnum;
    }

    /* At this point a_prev = A024167(n) and a_prev2 = A024167(n-1). */
    seq_t g_final = gcd(a_prev, a_prev2);
    return a_prev / g_final;
}
