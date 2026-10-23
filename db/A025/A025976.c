/*
 * A025976  Expansion of 1/((1-2*x)*(1-4*x)*(1-8*x)*(1-9*x)).
 * https://oeis.org/A025976
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 23*a(n-1) - 182*a(n-2) + 568*a(n-3) - 576*a(n-4)
   (n ≥ 4), with initial values a(0)=1, a(1)=23, a(2)=347, a(3)=4363. */
#include <stdint.h>

/* Compute A025976 term a(n) for any n ≥ 0.
   The function uses only integer arithmetic, is O(n) time, and
   requires no dynamic allocation. */
seq_t a(idx_t n)
{
    /* Initial terms */
    const seq_t a0 = (seq_t)1;
    const seq_t a1 = (seq_t)23;
    const seq_t a2 = (seq_t)347;
    const seq_t a3 = (seq_t)4363;

    if (n == 0) return a0;
    if (n == 1) return a1;
    if (n == 2) return a2;
    if (n == 3) return a3;

    /* Four previous values: a(n-4), a(n-3), a(n-2), a(n-1) */
    seq_t prev4 = a0;
    seq_t prev3 = a1;
    seq_t prev2 = a2;
    seq_t prev1 = a3;
    seq_t cur;

    for (idx_t i = 4; i <= n; ++i) {
        /* a(i) = 23*a(i-1) - 182*a(i-2) + 568*a(i-3) - 576*a(i-4) */
        cur = (seq_t)23 * prev1
            - (seq_t)182 * prev2
            + (seq_t)568 * prev3
            - (seq_t)576 * prev4;

        /* Shift previous values for next iteration */
        prev4 = prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
