/*
 * A035763  Coordination sequence for C_26 lattice.
 * https://oeis.org/A035763
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [x^(2n)] ((1+x)/(1-x))^26
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 13, u128 n <= 85,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    static seq_t binom26[27];
    static int initialized = 0;
    if (!initialized) {
        for (idx_t i = 0; i <= 26; ++i)
            binom26[i] = binom(26, i);
        initialized = 1;
    }

    idx_t two_n   = 2 * n;
    idx_t max_i   = two_n < 26 ? two_n : 26;

    seq_t sum = 0;
    for (idx_t i = 0; i <= max_i; ++i) {
        idx_t N    = 25 + two_n - i;
        seq_t term = binom26[i] * binom(N, 25);
        sum += term;
    }
    return sum;
}
