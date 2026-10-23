/*
 * A151961  Semiperimeter of the n-th Heronian triangle.
 * https://oeis.org/A151961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - a(n-2), a(1)=3, a(2)=6 (OEIS A151961)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function works for ordinary integer types as well as for the
   arbitrary‑precision integer class used in the big‑build.  It does
   not allocate any heap memory and therefore compiles cleanly in both
   C11 and C++17. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Offset is 1:  a(1)=3, a(2)=6.  For n==0 the return value
       is irrelevant but returning 0 avoids undefined behaviour. */
    if (n == 0)
        return 0;

    /* Handle the two initial terms directly. */
    if (n == 1)
        return (seq_t)3;
    if (n == 2)
        return (seq_t)6;

    /* Iteratively apply the recurrence a(n)=4*a(n-1)-a(n-2). */
    seq_t a_nm2 = (seq_t)3;   /* a(n-2) */
    seq_t a_nm1 = (seq_t)6;   /* a(n-1) */
    seq_t a_n   = 0;
    for (idx_t i = 3; i <= n; ++i) {
        a_n   = a_nm1 * (seq_t)4 - a_nm2;  /* a(n) = 4*a(n-1) - a(n-2) */
        a_nm2 = a_nm1;                     /* shift indices for next step */
        a_nm1 = a_n;
    }
    return a_n;
}
