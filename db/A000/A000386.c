/*
 * A000386  a(n) = A058057(n,n-3).
 * https://oeis.org/A000386
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -Sum_{k=3..n} (-1)^k * (n-k)! * binomial(k,3) *
 *            binomial(2*n-k,k)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static int fixed_flag = -1;

static bool seq_fixed(void) {
    if (fixed_flag < 0) {
        seq_t x = 1;
        int i;
        for (i = 0; i < 200; i++) x *= 2;
        fixed_flag = (x == 0) ? 1 : 0;
    }
    return fixed_flag == 1;
}

seq_t a(idx_t n) {
    if (n < 3) return 0;
    bool fixed = seq_fixed();
    seq_t P = 0, N = 0;          /* Horner accumulators for + and - parts */
    seq_t Bs = 0;                /* binomial(2n-k,k), exact (big mode) */
    uint64_t Bu = 0;             /* binomial(2n-k,k), small (fixed mode) */
    idx_t m0 = 2 * n - 3;
    if (fixed) {
        Bu = (uint64_t)m0 * (m0 - 1) * (m0 - 2) / 6;
    } else {
        Bs = (seq_t)m0 * (seq_t)(m0 - 1) * (seq_t)(m0 - 2) / 6;
    }
    idx_t k;
    for (k = 3; k <= n; k++) {
        if (k > 3) {
            idx_t mul = n - k + 1;
            P *= mul;
            N *= mul;
        }
        uint64_t c3 = (uint64_t)k * (k - 1) * (k - 2) / 6;
        seq_t B = fixed ? (seq_t)Bu : Bs;
        seq_t h = (seq_t)c3 * B;
        if (k & 1) N += h; else P += h;
        if (k < n) {
            idx_t num = (2 * n - 2 * k) * (2 * n - 2 * k - 1);
            idx_t den = (k + 1) * (2 * n - k);
            if (fixed) {
                Bu = Bu * num / den;
            } else {
                Bs = Bs * (seq_t)num / (seq_t)den;
            }
        }
    }
    return N - P;
}
