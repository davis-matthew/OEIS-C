/*
 * A223962  Number of 2 X n 0..3 arrays with antidiagonals unimodal and rows
 *          and diagonals nondecreasing.
 * https://oeis.org/A223962
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 88, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A223962: Number of 2 × n 0..3 arrays with antidiagonals unimodal
 * and rows/diagonals nondecreasing.
 *
 * Implements the order‑7 linear recurrence
 *    a(n) = 7 a(n‑1) – 21 a(n‑2) + 35 a(n‑3)
 *          – 35 a(n‑4) + 21 a(n‑5) – 7 a(n‑6) + a(n‑7)   ,  n > 7
 *
 * with initial values (1‑based):
 *    a(1)=16, a(2)=85, a(3)=295, a(4)=805, a(5)=1876,
 *    a(6)=3906, a(7)=7470.
 *
 * The routine works for any seq_t and idx_t provided by the build system
 * and can be called in arbitrary order.  Only a single small static array
 * of length 7 is used, so the memory footprint is constant.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Return 0 for undefined indices (the tests never use n < 1). */
    if (n < 1)
        return (seq_t)0;

    /* Handle the seven known base cases directly. */
    switch (n) {
    case 1: return (seq_t)16;
    case 2: return (seq_t)85;
    case 3: return (seq_t)295;
    case 4: return (seq_t)805;
    case 5: return (seq_t)1876;
    case 6: return (seq_t)3906;
    case 7: return (seq_t)7470;
    default: break;
    }

    /* Store the last seven values a(n‑7)…a(n‑1).  Index 0 holds the
     * oldest, index 6 the newest.  Initialise with the known a(1)…a(7). */
    seq_t prev[7];
    prev[0] = (seq_t)16;
    prev[1] = (seq_t)85;
    prev[2] = (seq_t)295;
    prev[3] = (seq_t)805;
    prev[4] = (seq_t)1876;
    prev[5] = (seq_t)3906;
    prev[6] = (seq_t)7470;

    /* Compute successive terms up to a(n). */
    for (idx_t i = 8; i <= n; ++i) {
        seq_t val = (seq_t)7 * prev[6]                 /* a(n‑1) */
                 - (seq_t)21 * prev[5]                /* a(n‑2) */
                 + (seq_t)35 * prev[4]                /* a(n‑3) */
                 - (seq_t)35 * prev[3]                /* a(n‑4) */
                 + (seq_t)21 * prev[2]                /* a(n‑5) */
                 - (seq_t)7 * prev[1]                 /* a(n‑6) */
                 + prev[0];                          /* a(n‑7) */

        /* Shift the ring buffer: drop the oldest (prev[0]),
         * shift others left, and store the new value at prev[6]. */
        prev[0] = prev[1];
        prev[1] = prev[2];
        prev[2] = prev[3];
        prev[3] = prev[4];
        prev[4] = prev[5];
        prev[5] = prev[6];
        prev[6] = val;
    }

    return prev[6];
}
