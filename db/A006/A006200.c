/*
 * A006200  Number of partitions into pairs.
 * https://oeis.org/A006200
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A079267(n+2, 3) = Sum_{j=3..N} (-1)^(j-3) * C(j,3) *
 *            C(2N-j,j) * (2N-2j-1)!!, N = n+2 (perfect matchings of 2N points
 *            on a line with exactly 3 short pairs, by inclusion-exclusion).
 *            g_j = C(2N-j,j)*(2N-2j-1)!! is computed downward: g_N = 1,
 *            g_{j-1} = g_j*(2N-j+1)*j / (2*(N-j+1))
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   a_u128, seq_is_big, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_is_big.h"
#include "../utils/u128_to_seq.h"
#include "../utils/a_u128.h"

static seq_t a_big(idx_t n) {
    idx_t N = n + 2;
    seq_t g = 1, pos = 0, neg = 0;
    idx_t j;
    for (j = N; j >= 3; j--) {
        seq_t c3 = (seq_t)j * (seq_t)(j - 1) * (seq_t)(j - 2) / 6;
        if (((j - 3) & 1) == 0) pos += c3 * g;
        else neg += c3 * g;
        if (j == 3) break;
        g = g * (seq_t)(2 * N - j + 1) * (seq_t)j / (seq_t)(2 * (N - j + 1));
    }
    return pos - neg;
}

seq_t a(idx_t n) {
    if (seq_is_big()) return a_big(n);
    return u128_to_seq(a_u128(n));
}
