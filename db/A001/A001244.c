/*
 * A001244  Eulerian numbers (Euler's triangle: column k=8 of A008292, column
 *          k=7 of A173018).
 * https://oeis.org/A001244
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{j=0..7} (-1)^j*binomial(n+1,j)*(8-j)^n, for n >= 8
 * Range:     u8 n <= 8, u16 n <= 10, u32 n <= 13, u64 n <= 22, u128 n <= 42,
 *            big any n
 * Helpers:   a001244_pw
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a001244_pw.h"

seq_t a(idx_t n)
{
    seq_t pos = 0, neg = 0;
    int j;
    if (n < 8) return 0;
    for (j = 0; j <= 7; j++) {
        seq_t binom;
        seq_t term;
        int i;
        if (n <= 1000) {
            uint64_t b = 1;
            for (i = 0; i < j; i++)
                b = b * (uint64_t)(n + 1 - (idx_t)i) / (uint64_t)(i + 1);
            binom = (seq_t)b;
        } else {
            binom = 1;
            for (i = 0; i < j; i++)
                binom = binom * ((seq_t)n + 1 - i) / (i + 1);
        }
        term = binom * a001244_pw((seq_t)(8 - j), n);
        if (j & 1) neg += term;
        else pos += term;
    }
    return pos - neg;
}
