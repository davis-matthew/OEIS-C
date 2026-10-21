/*
 * A082480  a(n) = Product_{k=1..n} (F(k)+1) where F(k) denotes the k-th
 *          Fibonacci number.
 * https://oeis.org/A082480
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=1..n} (F(k)+1), with the Fibonacci numbers
 *            F(1)=F(2)=1, and a(0)=1. The recurrence F(k)=F(k-1)+F(k-2) is
 *            used to generate the terms sequentially; the product is
 *            accumulated in the variable 'res'. This works for any n ≥ 0 and
 *            for every build of seq_t (arbitrary‑precision or native unsi...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 10, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* a(0)=1 by definition */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Initial Fibonacci values: F(0)=0, F(1)=1 */
    seq_t f_prev = (seq_t)0;   /* F(k-2) */
    seq_t f_cur  = (seq_t)1;   /* F(k-1) */
    seq_t res    = (seq_t)1;   /* product of (F(k)+1) up to current k */

    /* k=1: use F(1)=1, multiply by (1+1)=2 */
    res = res * ((seq_t)2);    /* F(1)=1 → F(1)+1 = 2 */

    /* iterate k=2 .. n */
    for (idx_t k = 2; k <= n; ++k) {
        seq_t f_next = f_prev + f_cur; /* F(k) = F(k-1) + F(k-2) */
        f_prev = f_cur;
        f_cur  = f_next;
        res = res * (f_cur + (seq_t)1);
    }

    return res;
}
