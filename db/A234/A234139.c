/*
 * A234139  Number of (n+1) X (7+1) 0..3 arrays with every 2 X 2 subblock
 *          having the sum of the absolute values of all six edge and diagonal
 *          differences equal to 9.
 * https://oeis.org/A234139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 8*a(n-2) - 30*a(n-3) + 4*a(n-4) + 48*a(n-5) -
 *            32*a(n-6) with initial values: a(1)=9216, a(2)=9992, a(3)=11336,
 *            a(4)=14120, a(5)=19368, a(6)=30632 (OEIS offset 1)
 * Range:     u8 none, u16 n <= 7, u32 n <= 19, u64 n <= 40, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* Initial six terms of the sequence, zero‑based for convenience. */
static const seq_t init[6] = {
    (seq_t)9216,
    (seq_t)9992,
    (seq_t)11336,
    (seq_t)14120,
    (seq_t)19368,
    (seq_t)30632
};

/* Compute the n-th term of A234139.  n >= 1.
   If n <= 6, return the directly known value.  For n > 6, iterate
   the linear recurrence using only the six most recent terms, so
   no dynamic allocation or memoisation is required. */
seq_t a(idx_t n)
{
    if (n < 1)               /* undefined for n < 1 – return 0 as dummy */
        return (seq_t)0;

    if (n <= 6)              /* return pre–computed initial value */
        return init[n - 1];

    /* Variables holding a(n-6) … a(n-1) for the current window. */
    seq_t t1 = init[0];
    seq_t t2 = init[1];
    seq_t t3 = init[2];
    seq_t t4 = init[3];
    seq_t t5 = init[4];
    seq_t t6 = init[5];

    seq_t next;
    for (idx_t i = 7; i <= n; ++i) {
        /* Use the recurrence to compute a(i). */
        next =  (seq_t)3 * t6
             + (seq_t)8 * t5
             - (seq_t)30 * t4
             + (seq_t)4 * t3
             + (seq_t)48 * t2
             - (seq_t)32 * t1;

        /* Shift the window for the next iteration. */
        t1 = t2;
        t2 = t3;
        t3 = t4;
        t4 = t5;
        t5 = t6;
        t6 = next;
    }

    return t6;               /* a(n) now resides in t6 */
}
