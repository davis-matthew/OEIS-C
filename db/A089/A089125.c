/*
 * A089125  a(n+2) = a(n+1) + F(n+1)*a(n), where F = Fibonacci number
 *          (A000045) and a(0) = a(1) = 1.
 * https://oeis.org/A089125
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 14, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n+2) = a(n+1) + F(n+1)*a(n), with a(0) = a(1) = 1
   where F(k) is the k‑th Fibonacci number (F(0)=0, F(1)=1). */
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;

    /* Fibonacci numbers: f0 = F(0), f1 = F(1) */
    seq_t f0 = (seq_t)0;
    seq_t f1 = (seq_t)1;

    /* a(n-2) and a(n-1) */
    seq_t prev2 = (seq_t)1;  /* a(0) */
    seq_t prev1 = (seq_t)1;  /* a(1) */
    seq_t curr;

    for (idx_t i = 2; i <= n; ++i) {
        /* Use F(i-1) = f1 */
        curr = prev1 + f1 * prev2;

        /* Shift previous values */
        prev2 = prev1;
        prev1 = curr;

        /* Advance Fibonacci numbers: compute F(i) */
        seq_t f2 = f0 + f1;   /* f2 = F(i)   */
        f0 = f1;              /* f0 = F(i-1) */
        f1 = f2;              /* f1 = F(i)   */
    }

    return prev1;  /* a(n) */
}
