/*
 * A218478  Number of 3n-length 8-ary words, either empty or beginning with
 *          the first letter of the alphabet, that can be built by repeatedly
 *          inserting triples of identical letters into the initially empty
 *          word.
 * https://oeis.org/A218478
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1/n * Sum_{j=0..n-1} C(3n,j)*(n-j)*7^j for n>0, a(0) = 1.
 *            Recurrence: ... (same as OEIS)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    /* If seq_t is 64‑bit or less, use 128‑bit intermediates to
       avoid overflow; otherwise rely on seq_t’s arbitrary‑precision
       operations. */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 sum128  = 0;
        unsigned __int128 binom128 = 1;
        unsigned __int128 pow7     = 1;
        idx_t threeN = 3 * n;

        for (idx_t j = 0; j < n; ++j) {
            unsigned __int128 term = binom128;
            term *= (unsigned __int128)(n - j);
            term *= pow7;
            sum128 += term;

            if (j + 1 < n) {
                unsigned __int128 num = binom128 *
                                        (unsigned __int128)(threeN - j);
                binom128 = num / (unsigned __int128)(j + 1);
            }
            pow7 *= 7;
        }
        return (seq_t)(sum128 / (unsigned __int128)n);
    } else {
        seq_t sum = 0;
        seq_t binom = 1;
        seq_t pow7  = 1;
        idx_t threeN = 3 * n;

        for (idx_t j = 0; j < n; ++j) {
            seq_t term = binom;
            term *= (seq_t)(n - j);
            term *= pow7;
            sum += term;

            if (j + 1 < n) {
                seq_t num = binom * (seq_t)(threeN - j);
                binom = num / (seq_t)(j + 1);
            }
            pow7 *= 7;
        }
        return sum / (seq_t)n;
    }
}
