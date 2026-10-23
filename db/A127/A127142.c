/*
 * A127142  First matching value found in A127141.
 * https://oeis.org/A127142
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * First repeated value in the iterates of 
 *   f(x) =  x/2            if x is even
 *          3*x + (2*n+1)   if x is odd
 * starting from x = 1.
 * The function uses Floyd's cycle‑finding algorithm.
 */
#include <stdint.h>

static inline seq_t f_step(seq_t x, idx_t n)
{
    if ((x & (seq_t)1) == 0)           /* even */
        return x / (seq_t)2;
    else                               /* odd */
        return (seq_t)3 * x + ((seq_t)2 * (seq_t)n + (seq_t)1);
}

seq_t a(idx_t n)
{
    /* Floyd's cycle detection */
    seq_t tortoise = f_step((seq_t)1, n);          /* x1 */
    seq_t hare     = f_step(f_step((seq_t)1, n), n);/* x2 */

    while (tortoise != hare) {
        tortoise = f_step(tortoise, n);
        hare     = f_step(f_step(hare, n), n);
    }

    /* Find the entry point of the cycle (first repeated value) */
    tortoise = (seq_t)1;
    while (tortoise != hare) {
        tortoise = f_step(tortoise, n);
        hare     = f_step(hare, n);
    }

    return tortoise;              /* first repeated value */
}
