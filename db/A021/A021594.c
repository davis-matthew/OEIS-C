/*
 * A021594  Expansion of 1/((1-x)(1-3x)(1-7x)(1-9x)).
 * https://oeis.org/A021594
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A021594: Expansion of 1/((1-x)(1-3x)(1-7x)(1-9x))
 * Recurrence (for n ≥ 4):
 *   a(n) = 20*a(n-1) - 130*a(n-2) + 300*a(n-3) - 189*a(n-4)
 * Initial values: a(0)=1, a(1)=20, a(2)=270, a(3)=3100
 */

#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* Handle the first four terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)20;
    if (n == 2) return (seq_t)270;
    if (n == 3) return (seq_t)3100;

    /* Iterative computation of the recurrence */
    seq_t a0 = (seq_t)1;     /* a(0) */
    seq_t a1 = (seq_t)20;    /* a(1) */
    seq_t a2 = (seq_t)270;   /* a(2) */
    seq_t a3 = (seq_t)3100;  /* a(3) */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t val = mul_small(a3, 20u);          /* 20 * a(n-1) */
        val -= mul_small(a2, 130u);             /* -130 * a(n-2) */
        val += mul_small(a1, 300u);             /* +300 * a(n-3) */
        val -= mul_small(a0, 189u);             /* -189 * a(n-4) */

        /* Shift the window forward */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = val;
    }

    return a3;
}
