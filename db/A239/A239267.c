/*
 * A239267  Number of domicule tilings of a 5 X 2n grid.
 * https://oeis.org/A239267
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies the linear recurrence a(n) = 145 a(n‑1) – 4394
 *            a(n‑2) + 46976 a(n‑3) – 201552 a(n‑4) + 352480 a(n‑5) – 180608
 *            a(n‑6) – 58112 a(n‑7) + 16384 a(n‑8) with the initial values
 *            a(0)=1, a(1)=21, a(2)=1563, a(3)=162409, a(4)=17508475,
 *            a(5)=1894621633, a(6)=205109410835, a(7)=22206188455913
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t init[8] = {
    (seq_t)1,
    (seq_t)21,
    (seq_t)1563,
    (seq_t)162409,
    (seq_t)17508475,
    (seq_t)1894621633,
    (seq_t)205109410835,
    (seq_t)22206188455913
};

seq_t a(idx_t n)
{
    /* Handle the first eight known terms directly. */
    if (n < 8) {
        return init[(size_t)n];
    }

    /* Working array holding the last eight values of the sequence. */
    seq_t last[8];
    /* Initialise it with the first eight values. */
    for (size_t i = 0; i < 8; ++i) {
        last[i] = init[i];
    }

    /* Iterate from n = 8 up to the requested n. */
    for (size_t i = 8; i <= (size_t)n; ++i) {
        /* Compute the next value using the recurrence. */
        seq_t pos = last[7] * (seq_t)145          /* a(n-1) */
                 +   last[5] * (seq_t)46976        /* a(n-3) */
                 +   last[3] * (seq_t)352480       /* a(n-5) */
                 +   last[0] * (seq_t)16384;      /* a(n-8) */

        seq_t neg = last[6] * (seq_t)4394          /* a(n-2) */
                 +   last[4] * (seq_t)201552      /* a(n-4) */
                 +   last[2] * (seq_t)180608      /* a(n-6) */
                 +   last[1] * (seq_t)58112;      /* a(n-7) */

        /* The recurrence guarantees that pos >= neg, so unsigned subtraction
           yields the correct non‑negative result. */
        seq_t next = pos - neg;

        /* Shift the window one step forward. */
        for (size_t k = 0; k < 7; ++k) {
            last[k] = last[k + 1];
        }
        last[7] = next;
    }

    /* last[7] now holds a(n). */
    return last[7];
}
