/*
 * A000856  Number of n-input 2-output switching networks under the action of
 *          S(n) on the inputs and S(2) and complementing group C(2,2) on the
 *          outputs.
 * https://oeis.org/A000856
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Burnside count over S(n) x (output group of order 8). For sigma
 *            in S(n), let C = number of cycles of sigma acting on {0,1}^n and
 *            E = number of those cycles of even length. Every
 *            fixed-point-free output element fixes nothing, because the
 *            all-zero input is a fixed point of sigma. So the elements fi...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   a856_gcd, a856_leaf, a856_pow2, a856_rec
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/a856_gcd.h"
#include "../utils/a856_leaf.h"
#include "../utils/a856_pow2.h"
#include "../utils/a856_rec.h"

seq_t a(idx_t n) {
    if (n == 0 || n > 62) return 0;
    int N = (int)n;
    seq_t fact = 1;
    for (int i = 2; i <= N; i++) fact *= (seq_t)i;
    if (fact == 0) return 0;
    int *parts = (int *)malloc((size_t)N * sizeof(int));
    seq_t T = 0;
    a856_rec(N, N, parts, 0, fact, &T);
    free(parts);
    return T / fact;
}
