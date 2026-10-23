/*
 * A194851  List of the 14 blocks in the Reed-Muller version of the Steiner
 *          System S(3,4,8) written as decimal numbers.
 * https://oeis.org/A194851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: generate all weight‑4 codewords of the extended Hamming [8,4]
   code (RM(1,3)), sort them ascending, and return the 1‑indexed value. */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* static table of the 14 codewords computed once */
    static seq_t vals[14];
    static int ready = 0;
    if (!ready) {
        size_t k = 0;
        for (idx_t avec = 1; avec <= 7; ++avec) {
            for (int b = 0; b < 2; ++b) {
                seq_t w = 0;
                for (idx_t i = 0; i < 8; ++i) {
                    idx_t andbits = avec & i;
                    int dot = ((andbits & 1) ^ ((andbits >> 1) & 1) ^ ((andbits >> 2) & 1));
                    int bit = dot ^ b;
                    if (bit)
                        w |= ((seq_t)1 << (7 - i));
                }
                vals[k++] = w;
            }
        }
        /* sort the 14 values ascending (simple insertion sort) */
        for (size_t i = 0; i < 14; ++i) {
            for (size_t j = i + 1; j < 14; ++j) {
                if (vals[j] < vals[i]) {
                    seq_t t = vals[i];
                    vals[i] = vals[j];
                    vals[j] = t;
                }
            }
        }
        ready = 1;
    }
    if (n >= 1 && n <= 14)
        return vals[n - 1];
    return 0;
}
