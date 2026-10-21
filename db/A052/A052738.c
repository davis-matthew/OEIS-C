/*
 * A052738  A simple context-free grammar in a labeled universe: labeled
 *          version of A052708.
 * https://oeis.org/A052738
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n!*A052708(n) Recurrence: a(0)=0, a(1)=0, a(2)=2,
 *            a(3)=12, a(4)=120 (6144*n^4+9216*n^3+1152*n^2-2496*n-576)*a(n)
 *            +(-9096-30320*n-28032*n^2-7936*n^3)*a(n+1)
 *            +(15488*n+6272*n^2+10404)*a(n+2) +(-3784*n-7600)*a(n+3)
 *            +589*a(n+4)=0
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* small offsets */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)12;
    if (n == 4) return (seq_t)120;

    /* use a sliding window of the last four values */
    seq_t a_n  = (seq_t)0;    /* a(n)   */
    seq_t a_n1 = (seq_t)2;    /* a(n+1) */
    seq_t a_n2 = (seq_t)12;   /* a(n+2) */
    seq_t a_n3 = (seq_t)120;  /* a(n+3) */
    seq_t next;

    /* loop from n = 1 to n = target-4 inclusive */
    for (idx_t n_val = 1; n_val <= n - 4; ++n_val)
    {
        /* cast n to __int128 for safe arithmetic */
        __int128 tn = n_val;

        /* coefficients (signed) */
        __int128 c0 = 6144*tn*tn*tn*tn + 9216*tn*tn*tn
                     + 1152*tn*tn - 2496*tn - 576;
        __int128 c1 = -9096 - 30320*tn - 28032*tn*tn
                     - 7936*tn*tn*tn;
        __int128 c2 = 15488*tn + 6272*tn*tn + 10404;
        __int128 c3 = -3784*tn - 7600;

        /* compute the left side of the recurrence */
        __int128 term = c0*(__int128)a_n
                      + c1*(__int128)a_n1
                      + c2*(__int128)a_n2
                      + c3*(__int128)a_n3;

        /* a(n+4) = -term / 589 */
        next = (seq_t)(-term / 589);

        /* shift the window forward */
        a_n  = a_n1;
        a_n1 = a_n2;
        a_n2 = a_n3;
        a_n3 = next;
    }
    return next;   /* a(n) has been computed */
}
