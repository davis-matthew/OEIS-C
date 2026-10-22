/*
 * A003181  Number of P-equivalence classes of nondegenerate Boolean functions
 *          of n variables.
 * https://oeis.org/A003181
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003180(n) - A003180(n-1) for n >= 1, a(0) = A003180(0) =
 *            2, where A003180(n) = (1/n!) * Sum_{partitions lambda of n}
 *            (n!/z_lambda) * 2^c(lambda), with c(lambda) = number of cycles
 *            of a permutation of cycle type lambda acting on the 2^n subsets
 *            of {1..n}: c = (1/L) * Sum_{j=0}^{L-1} 2^(Sum...
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   a3181_F, a3181_big, a3181_g, a3181_gen
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/a3181_F.h"
#include "../utils/a3181_big.h"
#include "../utils/a3181_g.h"
#include "../utils/a3181_gen.h"

seq_t a(idx_t n)
{
    int big = a3181_big();
    int m = (int)n;
    if (n > 20) return 0;
    if (big) {
        seq_t f1 = 0, f0 = 0;
        a3181_F(m, 1, &f1);
        if (m == 0) return f1;
        a3181_F(m - 1, 1, &f0);
        return f1 - f0;
    } else {
        seq_t dummy = 0, r;
        unsigned __int128 v = a3181_F(m, 0, &dummy);
        if (m > 0) v -= a3181_F(m - 1, 0, &dummy);
        r = (seq_t)(uint64_t)(v >> 64);
        r = (seq_t)(r * (seq_t)4294967296ULL);
        r = (seq_t)(r * (seq_t)4294967296ULL);
        r = (seq_t)(r + (seq_t)(uint64_t)v);
        return r;
    }
}
