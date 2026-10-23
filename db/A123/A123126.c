/*
 * A123126  Absolute value of coefficient of X^2 in the characteristic
 *          polynomial of the n-th power of the matrix M = {{1,1,1,1,1},
 *          {1,0,0,0,0}, {0,1,0,0,0}, {0,0,1,0,0}, {0,0,0,1,0}}.
 * https://oeis.org/A123126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            a(n-1)+a(n-3)-a(n-4)+6a(n-5)-3a(n-6)-3a(n-7)-2a(n-8)-a(n-9)-a(n-10)
 *            (order‑10 linear recurrence with the initial values a(1)..a(10)
 *            listed below)
 * Range:     u8 n <= 14, u16 n <= 28, u32 n <= 55, u64 n <= 110,
 *            u128 n <= 221, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Precomputed first ten terms of the sequence. */
static const seq_t base[10] = {
    (seq_t)1, (seq_t)1, (seq_t)4, (seq_t)1, (seq_t)31,
    (seq_t)22, (seq_t)1, (seq_t)33, (seq_t)4, (seq_t)141
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;       /* the sequence is defined for n ≥ 1 */

    /* Return the pre‑computed value for the first ten indices. */
    if (n <= 10) return base[n - 1];

    /* Sliding window that stores the last ten computed values:
       win[0] = a(i-10), win[1] = a(i-9), … , win[9] = a(i-1)   */
    seq_t win[10];
    for (size_t j = 0; j < 10; ++j) win[j] = base[j];

    /* Compute terms a(11) … a(n) iteratively. */
    for (idx_t i = 11; i <= n; ++i) {
        /* Map each needed past value to the corresponding window index. */
        seq_t a1 = win[9];   /* a(i-1)   */
        seq_t a2 = win[7];   /* a(i-3)   */
        seq_t a3 = win[6];   /* a(i-4)   */
        seq_t a4 = win[5];   /* a(i-5)   */
        seq_t a5 = win[4];   /* a(i-6)   */
        seq_t a6 = win[3];   /* a(i-7)   */
        seq_t a7 = win[2];   /* a(i-8)   */
        seq_t a8 = win[1];   /* a(i-9)   */
        seq_t a9 = win[0];   /* a(i-10)  */

        /* Compute the positive and negative contributions separately
           to avoid temporary negative values.          */
        seq_t pos = a1 + a2;
        pos += a4 * (seq_t)6;

        seq_t neg = a3;
        neg += a5 * (seq_t)3;
        neg += a6 * (seq_t)3;
        neg += a7 * (seq_t)2;
        neg += a8;
        neg += a9;

        seq_t term = pos - neg;   /* guaranteed non‑negative */

        /* Shift the window left and insert the new term. */
        for (size_t j = 0; j < 9; ++j) win[j] = win[j + 1];
        win[9] = term;
    }

    return win[9];  /* win[9] contains a(n) after the loop finishes. */
}
