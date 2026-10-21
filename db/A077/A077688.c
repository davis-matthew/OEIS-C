/*
 * A077688  a(n) = sum of all cyclic permuted concatenations of the next n
 *          numbers.
 * https://oeis.org/A077688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   for n such that all numbers in the block have equal length L *
 *            a(n) = ((n^2 + 1) * n) / 2 * (10^(L*n) - 1) / (10^L - 1) * For
 *            block lengths that cross a power‑of‑10 boundary the above
 *            formula * is not applicable, and the sequence is computed by the
 *            * direct (brute‑force) construction of every cyc...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n)
{
    /*  n >= 1 by definition of the sequence */
    idx_t start = n * (n - 1) / 2 + 1;
    idx_t last  = start + n - 1;

    int L_start = digit_len((seq_t)start);
    int L_last  = digit_len((seq_t)last);

    if (L_start == L_last) {
        /* All n numbers have the same length L = L_start */
        seq_t nn     = (seq_t)n;
        seq_t term1  = ((nn * nn + (seq_t)1) * nn) / (seq_t)2;
        seq_t powLn  = pow10((idx_t)(L_start * n));
        seq_t powL   = pow10((idx_t)L_start);
        seq_t term2  = (powLn - (seq_t)1) / (powL - (seq_t)1);
        return term1 * term2;
    }

    /* Brute force for blocks that cross a power‑of‑10 boundary */
    unsigned char *lens = (unsigned char *)malloc((size_t)n);
    if (!lens) return (seq_t)0;          /* allocation failure */

    for (idx_t i = 0; i < n; ++i)
        lens[i] = (unsigned char)digit_len((seq_t)(start + i));

    seq_t sum = 0;
    for (idx_t k = 0; k < n; ++k) {
        seq_t concat = 0;
        for (idx_t off = 0; off < n; ++off) {
            idx_t idx = k + off;
            if (idx >= n) idx -= n;
            seq_t val  = (seq_t)(start + idx);
            int   lenv = lens[idx];
            concat = concat * pow10((idx_t)lenv) + val;
        }
        sum += concat;
    }

    free(lens);
    return sum;
}
