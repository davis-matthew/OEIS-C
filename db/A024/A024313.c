/*
 * A024313  a(n) = s(1)*t(n) + s(2)*t(n-1) + ... + s(k)*t(n+1-k), where k =
 *          floor((n+1)/2), s = (natural numbers >= 3), t = A023531.
 * https://oeis.org/A024313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2*n) = Lucas(2*n+3) + F(2*n+2) - Lucas(n+3) - (n+1)*F(n+2);
 *            a(2*n+1) = Lucas(2*n+4) + F(2*n+3) - Lucas(n+3) - (n+2)*F(n+2);
 *            where F(k) is the Fibonacci number and Lucas(k) = F(k-1)+F(k+1).
 *            Offset 1 (a(1) = 3)
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"   /* provides seq_t fib(idx_t) */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* outside OEIS offset – unused */

    idx_t m;
    seq_t term;

    if (n & 1U) {           /* n is odd: n = 2*m+1 */
        m = (n - 1) / 2;

        /* Lucas(2*m+4) = fib(2*m+3) + fib(2*m+5) */
        seq_t L_2m4 = fib(2*m + 3) + fib(2*m + 5);
        /* F(2*m+3) */
        seq_t F_2m3 = fib(2*m + 3);
        /* F(m+2) */
        seq_t F_m2  = fib(m + 2);
        /* Lucas(m+3) = fib(m+2) + fib(m+4) */
        seq_t L_m3  = fib(m + 2) + fib(m + 4);

        term = L_2m4 + F_2m3 - L_m3 - (seq_t)(m + 2) * F_m2;
    } else {                /* n is even: n = 2*m */
        m = n / 2;

        /* Lucas(2*m+3) = fib(2*m+2) + fib(2*m+4) */
        seq_t L_2m3 = fib(2*m + 2) + fib(2*m + 4);
        /* F(2*m+2) */
        seq_t F_2m2 = fib(2*m + 2);
        /* F(m+2) */
        seq_t F_m2  = fib(m + 2);
        /* Lucas(m+3) = fib(m+2) + fib(m+4) */
        seq_t L_m3  = fib(m + 2) + fib(m + 4);

        term = L_2m3 + F_2m2 - L_m3 - (seq_t)(m + 1) * F_m2;
    }

    return term;
}
