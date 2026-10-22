/*
 * A004097  Sum of digits of Bell numbers.
 * https://oeis.org/A004097
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A007953(A000110(n)), the decimal digit sum of the Bell
 *            number B(n); B(n) comes from the Bell triangle T(0,0) = 1,
 *            T(i,0) = T(i-1,i-1), T(i,j) = T(i,j-1) + T(i-1,j-1), B(n) =
 *            T(n,0)
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   limb_digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../utils/limb_digit_sum.h"

static uint32_t *memo_sums = NULL;
static idx_t memo_cnt = 0;

static void compute_upto(idx_t N) {
    /* upper bound on decimal digits of B(N+1): (0.792 m / ln(m+1))^m */
    double m = (double)(N + 2);
    double r = 0.792 * m / log(m + 1.0);
    double dig = (r > 1.0) ? m * log10(r) : 1.0;
    size_t S = (size_t)(dig * 1.05 / 9.0) + 4;
    size_t cnt = (size_t)N + 1;
    uint32_t *A = (uint32_t *)calloc(cnt * S, sizeof(uint32_t));
    uint32_t *B = (uint32_t *)calloc(cnt * S, sizeof(uint32_t));
    uint32_t *sums = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    size_t L = 1;
    A[0] = 1;
    sums[0] = 1;
    for (idx_t i = 1; i <= N; i++) {
        uint32_t *src = A + (size_t)(i - 1) * S;
        memcpy(B, src, L * sizeof(uint32_t));
        B[L] = 0;
        for (idx_t j = 1; j <= i; j++) {
            uint32_t *prev = B + (size_t)(j - 1) * S;
            uint32_t *up = A + (size_t)(j - 1) * S;
            uint32_t *dst = B + (size_t)j * S;
            uint32_t carry = 0;
            for (size_t k = 0; k <= L; k++) {
                uint32_t v = prev[k] + up[k] + carry;
                if (v >= 1000000000u) { v -= 1000000000u; carry = 1; }
                else carry = 0;
                dst[k] = v;
            }
        }
        if (B[(size_t)i * S + L] != 0) L++;
        uint32_t s = 0;
        for (size_t k = 0; k < L; k++) s += limb_digit_sum(B[k]);
        sums[i] = s;
        uint32_t *t = A; A = B; B = t;
    }
    free(A);
    free(B);
    free(memo_sums);
    memo_sums = sums;
    memo_cnt = N + 1;
}

seq_t a(idx_t n) {
    if (n >= memo_cnt) {
        idx_t target = n;
        if (memo_cnt > 0 && target < 2 * memo_cnt) target = 2 * memo_cnt;
        if (target < 32) target = 32;
        compute_upto(target);
    }
    seq_t r = 0;
    r += (seq_t)memo_sums[n];
    return r;
}
