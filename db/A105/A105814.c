/*
 * A105814  a(n) = n^2 + (n concatenated with n).
 * https://oeis.org/A105814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = n^2 + n*(10^d + 1) where d = number of decimal digits of
 *            n
 * Range:     u8 n <= 9, u16 n <= 99, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 61 terms through n = 9996 (2026-11-01)
 * Helpers:   num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/num_digits_idx.h"

static seq_t pow10(idx_t exp) {
    seq_t res = 1;
    for (idx_t i = 0; i < exp; ++i) res *= (seq_t)10;
    return res;
}

seq_t a(idx_t n)
{
    int d = num_digits_idx(n);                    /* number of digits of n */
    seq_t nseq = (seq_t)n;
    seq_t tenpow = pow10((idx_t)d);       /* 10^d */
    seq_t result = nseq * nseq + nseq * (tenpow + (seq_t)1);
    return result;
}
