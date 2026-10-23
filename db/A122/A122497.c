/*
 * A122497  Let f(S) denote the interchange of 1's and 2's in S. Let S_0 = 1,
 *          S_{N+1} = f(S_N).S_N, where the dot indicates concatenation.
 *          Sequence gives S_0.S_1.S_2.S_3....
 * https://oeis.org/A122497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = A059448(n) + 1
 * Range:     u8 n <= 8191, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This computes the n‑th term of the infinite concatenation
   S_0.S_1.S_2… where S_0 = 1 and S_{k+1} = f(S_k).S_k
   (f swaps 1↔2).  The result is always 1 or 2. */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;  /* undefined for n<1 */

    /* Locate the block S_k that contains the n‑th position. */
    idx_t cum = 0;          /* total length of all previous blocks   */
    idx_t len = 1;          /* length of the current block S_k       */
    while (cum + len < n) {
        cum += len;
        len <<= 1;
    }

    /* Position of n inside S_k (0‑based). */
    idx_t offset = n - cum - 1;

    /* Count how many times the position falls into the first half,
       which means the symbol is inverted (1↔2). */
    uint8_t invert = 0;      /* 0: no inversion, 1: inverted */
    while (len > 1) {
        idx_t half = len >> 1;
        if (offset < half) {
            invert ^= 1;    /* toggle inversion */
        } else {
            offset -= half; /* move to the second half */
        }
        len = half;
    }

    return invert ? (seq_t)2 : (seq_t)1;
}
