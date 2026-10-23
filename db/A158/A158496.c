/*
 * A158496  Expansion of (1-4x+x^2)/(1+x^2)^2.
 * https://oeis.org/A158496
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 62, i16 n <= 66, i32 n <= 66, i64 n <= 66, i128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A158496: Expansion of (1-4x+x^2)/(1+x^2)^2
   Recurrence: a(0)=1, a(1)=-4, a(2)=-1, a(3)=8,
                a(n) = -2*a(n-2) - a(n-4)   for n≥4
   (Equivalent closed form: a(n) = (n+3/2-(n+1/2)*(-1)^n)*(-1)^C(n+1,2))
*/
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the first four terms explicitly */
    if (n == 0) return (seq_t) 1;
    if (n == 1) return (seq_t)(-4);
    if (n == 2) return (seq_t)(-1);
    if (n == 3) return (seq_t) 8;

    /* Use a 5‑element circular buffer.
       v[0] = a(k-4), v[1] = a(k-3), v[2] = a(k-2), v[3] = a(k-1), v[4] = a(k) */
    seq_t v[5];
    v[0] = (seq_t) 1;   /* a(0) */
    v[1] = (seq_t)(-4); /* a(1) */
    v[2] = (seq_t)(-1); /* a(2) */
    v[3] = (seq_t) 8;   /* a(3) */
    v[4] = (seq_t) 0;   /* placeholder */

    seq_t cur = (seq_t) 0;
    for (idx_t i = 4; i <= n; ++i) {
        idx_t idx_cur  =  i      % 5;
        idx_t idx_m2   = (i - 2) % 5;
        idx_t idx_m4   = (i - 4) % 5;

        cur = - (seq_t)2 * v[idx_m2] - v[idx_m4];
        v[idx_cur] = cur;
    }
    return cur;
}
