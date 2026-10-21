/*
 * A094715  a(n) = Sum_{2*i+3*j=n, 0<=i<=n, 0<=j<=n} n!/( (2*i)!*(3*j)! ).
 * https://oeis.org/A094715
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=0, a(2)=1, a(3)=1, a(4)=1, a(5)=10. for n>=6:
 *            a(n)=3*a(n-2)+2*a(n-3)-3*a(n-4)+6*a(n-5)
 * Range:     u8 n <= 11, u16 n <= 18, u32 n <= 34, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t base[6] = {1, 0, 1, 1, 1, 10};

    /* Return the few pre‑computed initial terms directly. */
    if (n <= 5)
        return base[n];

    /* Initialise the last five values needed for the recurrence.  We
       keep them in the order:
       t0 = a(n-5), t1 = a(n-4), t2 = a(n-3), t3 = a(n-2), t4 = a(n-1).  */
    seq_t t0 = base[1];  /* a(1)   */
    seq_t t1 = base[2];  /* a(2)   */
    seq_t t2 = base[3];  /* a(3)   */
    seq_t t3 = base[4];  /* a(4)   */
    seq_t t4 = base[5];  /* a(5)   */
    seq_t an  = 0;       /* value to be computed */

    for (idx_t k = 6; k <= n; ++k) {
        /* a(k) = 3*a(k-2) + 2*a(k-3) + 6*a(k-5) - 3*a(k-4) */
        an = (seq_t)3 * t3 + (seq_t)2 * t2 + (seq_t)6 * t0 - (seq_t)3 * t1;

        /* shift the window: drop the oldest value and append the new a(k) */
        t0 = t1;
        t1 = t2;
        t2 = t3;
        t3 = t4;
        t4 = an;
    }

    return an;
}
