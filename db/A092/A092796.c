/*
 * A092796  Number of connected relations.
 * https://oeis.org/A092796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 32^n - 5*17^n - 10*11^n + 20*10^n + 30*8^n - 60*7^n +
 *            24*6^n
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   _powseq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/_powseq.h"

seq_t a(idx_t n) {
    /* Calculate each power once */
    seq_t p6  = _powseq((seq_t)6, n);
    seq_t p7  = _powseq((seq_t)7, n);
    seq_t p8  = _powseq((seq_t)8, n);
    seq_t p10 = _powseq((seq_t)10, n);
    seq_t p11 = _powseq((seq_t)11, n);
    seq_t p17 = _powseq((seq_t)17, n);
    seq_t p32 = _powseq((seq_t)32, n);

    /* Combine according to the formula */
    seq_t ans = p32
              - (seq_t)5 * p17
              - (seq_t)10 * p11
              + (seq_t)20 * p10
              + (seq_t)30 * p8
              - (seq_t)60 * p7
              + (seq_t)24 * p6;

    return ans;
}
