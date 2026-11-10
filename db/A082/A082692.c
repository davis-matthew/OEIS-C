/*
 * A082692  Partial sums of A082691.
 * https://oeis.org/A082692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} A082691(k); block lengths L_k = 3*2^k - 1,
 *            block sums T_k = 2*T_{k-1} + k + 2, T_0 = 3 (block 0 = 1,2)
 * Range:     u8 n <= 116, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 81 terms through n = 9819 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t acc = 0;
    while (n > 2) {
        idx_t Lp = 2;
        seq_t Tp = 3;
        idx_t p = 2;
        unsigned k = 1;
        idx_t L = 3 * p - 1;
        seq_t T = (seq_t)2 * Tp + (seq_t)(k + 2);
        while (L < n && p < ((idx_t)1 << 61)) {
            Lp = L;
            Tp = T;
            p <<= 1;
            k++;
            L = 3 * p - 1;
            T = (seq_t)2 * Tp + (seq_t)(k + 2);
        }
        if (n == L) return acc + T;
        acc += Tp;
        n -= Lp;
    }
    if (n == 0) return acc;
    if (n == 1) return acc + (seq_t)1;
    return acc + (seq_t)3;
}
