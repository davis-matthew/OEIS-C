/*
 * A006236  a(n) = n^(n-2)*(n+2)^(n-1).
 * https://oeis.org/A006236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^(n-2)*(n+2)^(n-1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Helpers:   a6236_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a6236_pow.h"

seq_t a(idx_t n) {
    if (n < 2) {
        return (seq_t)1;
    }
    seq_t nv = (seq_t)n;
    seq_t np2 = nv + 2;
    return a6236_pow(nv, n - 2) * a6236_pow(np2, n - 1);
}
