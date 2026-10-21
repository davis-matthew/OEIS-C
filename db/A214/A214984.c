/*
 * A214984  Array: T(m,n) = (F(m) + F(2*m) + ... + F(n*m))/F(m), by
 *          antidiagonals, where F = A000045 (Fibonacci numbers).
 * https://oeis.org/A214984
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(m,n) = (F(m)+F(2m)+...+F(nm))/F(m) * The sequence is taken in
 *            antidiagonal order. * Recurrence: let q_k = F(mk)/F(m). Then *
 *            q_0 = 0, q_1 = 1 * q_{k+1} = L(m)*q_k + s*q_{k-1} * where L(m) =
 *            2*F(m+1) - F(m) is the m‑th Lucas number and * s = +1 if m is
 *            odd, s = –1 if m is even. * (Because F(m(k+...
 * Range:     u8 n <= 23, u16 n <= 48, u32 n <= 96, u64 n <= 197,
 *            u128 n <= 388, big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Convert n>=1 to diagonal d, row index i (m), column index n_ */
    idx_t d = 2;
    while (((d - 1) * d) / 2 < n) ++d;          /* find diagonal d  */
    idx_t prev = ((d - 2) * (d - 1)) / 2;      /* total elements before d */
    idx_t m = n - prev;                        /* row index (m) */
    idx_t nn = d - m;                          /* column index (n) in T(m,n) */

    /* Calculate Lucas number L(m) = 2*F(m+1) - F(m) */
    seq_t Fm    = fib(m);
    seq_t Fm1   = fib(m + 1);
    seq_t L     = 2 * Fm1 - Fm;

    /* sign s: +1 if m odd, –1 if m even */
    bool m_odd = (m & 1U);
    seq_t sum = 0;            /* sum of q_k for k=1..nn   */
    seq_t q_prev = 0;         /* q_0                       */
    seq_t q_cur  = 1;         /* q_1                       */
    sum += q_cur;             /* include k = 1             */

    if (nn >= 2) {
        seq_t q_next;
        for (idx_t k = 2; k <= nn; ++k) {
            if (m_odd) {
                q_next = L * q_cur + q_prev;
            } else {
                q_next = L * q_cur - q_prev;
            }
            sum += q_next;
            q_prev = q_cur;
            q_cur  = q_next;
        }
    }
    return sum;
}
