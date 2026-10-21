/*
 * A092467  a(n) = Sum_{i+j+k=n, 0<=i,j,k<=n} (n+2k)!/(i! * j! * (3*k)!).
 * https://oeis.org/A092467
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} C(n+2k,3k) * 2^(n-k) Equivalent linear
 *            recurrence: a(0)=1, a(1)=3, a(2)=13,
 *            a(n)=7*a(n-1)-12*a(n-2)+8*a(n-3) (n≥3)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle first three indices directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)13;

    /* Iteratively compute using the recurrence */
    seq_t a0 = (seq_t)1;      /* a(n-3) */
    seq_t a1 = (seq_t)3;      /* a(n-2) */
    seq_t a2 = (seq_t)13;     /* a(n-1) */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t next = (seq_t)7 * a2   /* 7*a(n-1) */
                    - (seq_t)12 * a1  /* 12*a(n-2) */
                    + (seq_t)8 * a0;  /* 8*a(n-3) */
        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2;
}
