/*
 * A000514  Eulerian numbers (Euler's triangle: column k=6 of A008292, column
 *          k=5 of A173018).
 * https://oeis.org/A000514
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{j=0..6} (-1)^j * binomial(n+1,j) * (6-j)^n
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 13, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   a000248_pow, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a000248_pow.h"
#include "../utils/u128_to_seq.h"

seq_t a(idx_t n) {
    seq_t pos = 0, neg = 0;
    unsigned __int128 c = 1; /* binomial(n+1, j), exact for practical n */
    idx_t j;
    for (j = 0; j <= 6; j++) {
        if (j > 0) c = c * (unsigned __int128)(n + 2 - j) / (unsigned __int128)j;
        if (j == 6) break; /* (6-6)^n = 0 for n >= 1 */
        seq_t term = u128_to_seq(c) * a000248_pow((seq_t)(6 - j), n);
        if (j % 2 == 0) pos += term;
        else neg += term;
    }
    return pos - neg;
}
