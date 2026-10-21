/*
 * A086293  Sum of divisors of 7-smooth numbers.
 * https://oeis.org/A086293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = σ(A002473(n)) (sum of divisors of the nth 7‑smooth
 *            number)
 * Range:     u8 n <= 47, u16 n <= 409, u32 n <= 5194, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static buffers for the 7‑smooth sequence and its divisor sums */
    static seq_t *smooth   = NULL;   /* 1‑based: smooth[i] = i‑th 7‑smooth number */
    static seq_t *sigmaVals = NULL;
    static size_t alloc = 0;          /* current allocation size (>=2) */
    static size_t i2 = 1, i3 = 1, i5 = 1, i7 = 1; /* pointers for the 4 multiples */
    static size_t listSize = 0;       /* number of computed 7‑smooth numbers */

    /* first run: initialisation */
    if (!smooth) {
        alloc = 64;
        smooth    = (seq_t*)malloc(alloc * sizeof(seq_t));
        sigmaVals = (seq_t*)malloc(alloc * sizeof(seq_t));
        smooth[1]   = 1;
        sigmaVals[1] = 1;          /* σ(1) = 1 */
        listSize = 1;
    }

    /* generate terms until we have n of them */
    while (listSize < n) {
        /* grow buffers when needed */
        if (listSize + 1 >= alloc) {
            alloc <<= 1;
            smooth    = (seq_t*)realloc(smooth,    alloc * sizeof(seq_t));
            sigmaVals = (seq_t*)realloc(sigmaVals, alloc * sizeof(seq_t));
        }

        /* next candidates from the four pointers */
        seq_t c2 = smooth[i2] * 2;
        seq_t c3 = smooth[i3] * 3;
        seq_t c5 = smooth[i5] * 5;
        seq_t c7 = smooth[i7] * 7;

        /* smallest candidate */
        seq_t next = c2;
        if (c3 < next) next = c3;
        if (c5 < next) next = c5;
        if (c7 < next) next = c7;

        /* store the new 7‑smooth number */
        ++listSize;
        smooth[listSize] = next;

        /* compute σ(next) by repeated factorisation with 2,3,5,7 */
        seq_t sigma = 1;
        seq_t temp  = next;

        if ((temp % 2) == 0) {
            seq_t sum = 1, pow = 1;
            while ((temp % 2) == 0) {
                pow *= 2;
                sum += pow;
                temp /= 2;
            }
            sigma *= sum;
        }
        if ((temp % 3) == 0) {
            seq_t sum = 1, pow = 1;
            while ((temp % 3) == 0) {
                pow *= 3;
                sum += pow;
                temp /= 3;
            }
            sigma *= sum;
        }
        if ((temp % 5) == 0) {
            seq_t sum = 1, pow = 1;
            while ((temp % 5) == 0) {
                pow *= 5;
                sum += pow;
                temp /= 5;
            }
            sigma *= sum;
        }
        if ((temp % 7) == 0) {
            seq_t sum = 1, pow = 1;
            while ((temp % 7) == 0) {
                pow *= 7;
                sum += pow;
                temp /= 7;
            }
            sigma *= sum;
        }

        sigmaVals[listSize] = sigma;

        /* advance the pointers that produced the chosen candidate */
        if (c2 == next) ++i2;
        if (c3 == next) ++i3;
        if (c5 == next) ++i5;
        if (c7 == next) ++i7;
    }

    return sigmaVals[n];
}
