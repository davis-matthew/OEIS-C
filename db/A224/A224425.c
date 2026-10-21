/*
 * A224425  The Wiener index of the phenylacetylene dendrimer NSB[n], defined
 *          pictorially in the Z. Yarahmadi et al. reference.
 * https://oeis.org/A224425
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 0, u32 n <= 6, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = -9369 + 85725*2^n - 65772*4^n + 68121*n*4^n
 * (OEIS A224425 – Wiener index of the phenylacetylene dendrimer NSB[n])
 */

#include <stdint.h>
#include <stdbool.h>

/* Compute a(n) for the sequence A224425 */
seq_t a(idx_t n)
{
    /* Compute 2^n and 4^n as big integers using repeated multiplication.
     * This avoids undefined behaviour with large shift counts. */
    seq_t pow2 = 1;
    seq_t pow4 = 1;
    idx_t i;
    for (i = 0; i < n; ++i) {
        pow2 <<= 1;   /* multiply by 2 each iteration */
    }
    for (i = 0; i < n; ++i) {
        pow4 <<= 2;   /* multiply by 4 each iteration (2^2) */
    }

    seq_t nseq = (seq_t)n;

    /* a(n) = 85725*2^n - 65772*4^n + 68121*n*4^n - 9369 */
    seq_t result = (seq_t)85725 * pow2;
    result -= (seq_t)65772 * pow4;
    result += (seq_t)68121 * nseq * pow4;
    result -= 9369;           /* subtraction works correctly even for unsigned */

    return result;
}
