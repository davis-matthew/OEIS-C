/*
 * A123238  A polynomial of degree 130.
 * https://oeis.org/A123238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, less
 * Formula:   a(n) = 1 + n^8 + n^12 + n^18 + n^20 + n^27 + n^28 + n^30 + n^42
 *            + n^44 + n^45 + n^50 + n^52 + n^63 + n^66 + n^68 + n^70 + n^75 +
 *            n^76 + n^78 + n^92 + n^98 + n^99 + n^102 + n^105 + n^110 + n^114
 *            + n^116 + n^117 + n^124 + n^125 + n^130
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 1, u128 n <= 1,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n) {
    seq_t tn = (seq_t)n;
    seq_t res = 1;                              /* the constant term */

    static const int exponents[] = {
        8,12,18,20,27,28,30,42,44,45,50,52,
        63,66,68,70,75,76,78,92,98,99,102,105,
        110,114,116,117,124,125,130
    };
    const size_t cnt = sizeof(exponents) / sizeof(exponents[0]);

    for (size_t i = 0; i < cnt; ++i) {
        res += seq_pow(tn, exponents[i]);
    }
    return res;
}
