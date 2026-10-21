/*
 * A087423  a(n) = S(3*n,3)/S(n,3) where S(n,m) = Sum_{k=0..n}
 *          binomial(n,k)*Fibonacci(m*k).
 * https://oeis.org/A087423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n + (14+6*sqrt(5))^n + (14-6*sqrt(5))^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 16,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This sequence satisfies the linear recurrence   */
/* a(n) = 32*a(n-1) - 128*a(n-2) + 64*a(n-3) for n >= 4 */
/* with initial values a(1)=32, a(2)=768, a(3)=20672. */

#include <stddef.h>
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)32;
    if (n == 2) return (seq_t)768;
    if (n == 3) return (seq_t)20672;

    seq_t a1 = (seq_t)32;      /* a(n-3)   */
    seq_t a2 = (seq_t)768;     /* a(n-2)   */
    seq_t a3 = (seq_t)20672;   /* a(n-1)   */
    seq_t next;

    for (idx_t i = 4; i <= n; ++i) {
        seq_t t1 = mul_small(a3, 32);   /* 32*a(n-1) */
        seq_t t2 = mul_small(a2, 128);  /* 128*a(n-2) */
        seq_t t3 = mul_small(a1, 64);   /* 64*a(n-3) */

        next = t1 - t2 + t3;  /* a(n) */

        /* shift for next iteration */
        a1 = a2;
        a2 = a3;
        a3 = next;
    }

    return a3;
}
