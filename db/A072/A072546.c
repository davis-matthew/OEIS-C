/*
 * A072546  Sequence related to Fibonacci and "tribonacci" sequences : let
 *          f(1)=1, f(2)=n, f(k+2)=f(k+1)+f(k), t(1)=t(2)=t(3)=1,
 *          t(k+3)=t(k+2)+t(k+1)+t(k) (t(k)=A000213(k-1)); sequence gives the
 *          smallest value k = a(n) such that t(k)>f(k).
 * https://oeis.org/A072546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k such that t(k) > f(k) where f(1)=1, f(2)=n,
 *            f(k+2)=f(k+1)+f(k) and t(1)=t(2)=t(3)=1,
 *            t(k+3)=t(k+2)+t(k+1)+t(k)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Fibonacci sequence: f(1)=1, f(2)=n
       Tribonacci sequence: t(1)=t(2)=t(3)=1
       We iterate k starting from 3.  */
    unsigned __int128 f_prevprev = 1;          /* f(k-2)  */
    unsigned __int128 f_prev     = (unsigned __int128)n;  /* f(k-1)  */
    unsigned __int128 f_curr     = f_prevprev + f_prev;     /* f(k)    */

    unsigned __int128 t1 = 1;  /* t(k-2) */
    unsigned __int128 t2 = 1;  /* t(k-1) */
    unsigned __int128 t3 = 1;  /* t(k)   */
    idx_t k = 3;  /* current k value */

    /* Check k=3, then iterate.  */
    while (1) {
        if (t3 > f_curr) {
            return (seq_t)k;
        }

        /* Compute next values for k+1. */
        ++k;
        unsigned __int128 f_next = f_curr + f_prev;
        f_prevprev = f_prev;
        f_prev = f_curr;
        f_curr = f_next;

        unsigned __int128 t_next = t1 + t2 + t3;
        t1 = t2;
        t2 = t3;
        t3 = t_next;
    }
}
