/*
 * A106256  Numbers n such that 12*n^2 + 13 is a square.
 * https://oeis.org/A106256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1; a(2)=3; a(3)=14*a(1)+a(2); a(4)=14*a(2)+a(1);
 * a(n)=14*a(n-2)-a(n-4) for n>=5.
 * The function works for any idx_t in any order of calls and uses
 * only integer arithmetic permitted for seq_t. */

#include <stddef.h>  /* for size_t, if needed */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;

    seq_t a1 = (seq_t)1;           /* a(1)  */
    seq_t a2 = (seq_t)3;           /* a(2)  */
    seq_t a3 = (seq_t)14 * a1 + a2;/* a(3)  */
    if (n == 3) return a3;
    seq_t a4 = (seq_t)14 * a2 + a1;/* a(4)  */
    if (n == 4) return a4;

    /* t0=a(i-4), t1=a(i-3), t2=a(i-2), t3=a(i-1) for current i */
    seq_t t0 = a1, t1 = a2, t2 = a3, t3 = a4;

    for (idx_t i = 5; i <= n; ++i) {
        seq_t new_val = (seq_t)14 * t2 - t0; /* a(i)   */
        t0 = t1;
        t1 = t2;
        t2 = t3;
        t3 = new_val;
    }
    return t3;   /* a(n) */
}
