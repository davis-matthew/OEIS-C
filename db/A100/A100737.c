/*
 * A100737  Inverse modulo 2 binomial transform of 4^n.
 * https://oeis.org/A100737
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum_{k=0..n} (-1)^popcount(n-k) * (binomial(n,k) mod 2) *
 *            4^k
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    seq_t sum_pos = 0;
    seq_t sum_neg = 0;
    idx_t subset = n;
    while (1) {
        idx_t k = subset;
        idx_t diff = n - k;
        int parity = __builtin_parityll(diff);
        seq_t term = pow2((idx_t)(2 * k));  /* 4^k = 2^(2k) */
        if (parity == 0)
            sum_pos += term;
        else
            sum_neg += term;
        if (subset == 0) break;
        subset = (subset - 1) & n;
    }
    return sum_pos - sum_neg;
}
