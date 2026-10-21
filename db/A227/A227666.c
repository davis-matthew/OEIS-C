/*
 * A227666  Number of lattice paths from {n}^4 to {0}^4 using steps that
 *          decrement one component by 1 such that for each point
 *          (p_1,p_2,...,p_4) we have abs(p_{i}-p_{i+1}) <= 1.
 * https://oeis.org/A227666
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 40*a(n-1) - 89*a(n-2) + 220*a(n-3) for n>3 with a(0)=1,
 *            a(1)=24, a(2)=896, a(3)=33904
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Handle the first four terms explicitly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)24;
    if (n == 2) return (seq_t)896;
    if (n == 3) return (seq_t)33904;

    /* Starting values for the recurrence. */
    seq_t a1 = (seq_t)1;      /* a(n-3) */
    seq_t a2 = (seq_t)24;     /* a(n-2) */
    seq_t a3 = (seq_t)896;    /* a(n-1) */
    seq_t a4 = (seq_t)33904;  /* current a(n) */

    /* Iterate from n=4 up to the desired index. */
    for (idx_t i = 4; i <= n; ++i) {
        /* Compute next term: a(i) = 40*a(i-1) - 89*a(i-2) + 220*a(i-3) */
        /* The sum 40*a(i-1)+220*a(i-3) is always ≥ 89*a(i-2) for n>3,
           so the subtraction does not underflow for unsigned seq_t. */
        seq_t term1 = (seq_t)40 * a4;   /* 40*a(i-1) */
        seq_t term2 = (seq_t)89 * a3;   /* 89*a(i-2) */
        seq_t term3 = (seq_t)220 * a2;  /* 220*a(i-3) */
        seq_t next = (term1 + term3) - term2;

        /* Shift the window for the next iteration. */
        a2 = a3;
        a3 = a4;
        a4 = next;
    }
    return a4;
}
