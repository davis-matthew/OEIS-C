/*
 * A201500  Number of n X 3 0..1 arrays with every row and column running
 *          average nondecreasing rightwards and downwards, and the number of
 *          instances of each value within one of each other.
 * https://oeis.org/A201500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-2) - 2*a(n-6) + a(n-8) for n ≥ 9, with initial
 *            values a(1)=2, a(2)=2, a(3)=6, a(4)=5, a(5)=12, a(6)=8, a(7)=20,
 *            a(8)=13
 * Range:     u8 n <= 30, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    /* Static allocation up to a reasonable bound (≈2×10⁴). */
    static seq_t seq[20001];
    static idx_t max_calculated = 0;

    if (n < 1) return 0;  /* undefined for n<1, return 0 */

    /* Ensure we have computed up to n.  */
    if (n > max_calculated) {
        idx_t start = (max_calculated == 0) ? 1 : max_calculated + 1;
        /* Initialize the first 8 terms if not done yet. */
        if (max_calculated < 8) {
            start = 1;
            seq[1] = (seq_t)2;   /* a(1) */
            seq[2] = (seq_t)2;   /* a(2) */
            seq[3] = (seq_t)6;   /* a(3) */
            seq[4] = (seq_t)5;   /* a(4) */
            seq[5] = (seq_t)12;  /* a(5) */
            seq[6] = (seq_t)8;   /* a(6) */
            seq[7] = (seq_t)20;  /* a(7) */
            seq[8] = (seq_t)13;  /* a(8) */
            max_calculated = 8;
            start = 9;
        }
        for (idx_t i = start; i <= n; ++i) {
            /* recurrence: a(i) = 2*a(i-2) - 2*a(i-6) + a(i-8) */
            /* Compute in a safe order to avoid unsigned underflow.          */
            seq_t t1 = (seq_t)2 * seq[i - 2];
            seq_t t2 = (seq_t)2 * seq[i - 6];
            seq_t t3 = seq[i - 8];
            seq[i] = t1 + t3 - t2;
        }
        max_calculated = n;
    }
    return seq[n];
}
