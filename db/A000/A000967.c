/*
 * A000967  Sum of Fermat coefficients.
 * https://oeis.org/A000967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Round( Sum_{c=1..n} binomial(2n-c, c-1)/c ), computed
 *            exactly: each term is split as floor(B/c) + (B mod c)/c, the
 *            integer parts are summed in multi-limb arithmetic, and the small
 *            fractional parts in 64-bit fixed point. B is updated by B(c+1) =
 *            B(c)*(2n-2c+1)/(2n-c) * (2n-2c)/c
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 26, u64 n <= 50, u128 n <= 97,
 *            big any n
 * Helpers:   a967_add, a967_addsmall, a967_div, a967_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a967_add.h"
#include "../utils/a967_addsmall.h"
#include "../utils/a967_div.h"
#include "../utils/a967_mul.h"

seq_t a(idx_t n) {
    size_t L = (size_t)(n / 16) + 4;
    uint32_t *B = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *T = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *S = (uint32_t *)calloc(L, sizeof(uint32_t));
    unsigned __int128 F = 0;
    B[0] = 1; /* binomial(2n-1, 0) */
    for (idx_t c = 1; c <= n; c++) {
        memcpy(T, B, L * sizeof(uint32_t));
        uint32_t r = a967_div(T, L, (uint32_t)c);
        a967_add(S, T, L);
        F += (((unsigned __int128)r) << 64) / (uint32_t)c;
        if (c < n) {
            uint64_t m = 2 * n - c;
            a967_mul(B, L, (uint32_t)(m - c + 1));
            a967_div(B, L, (uint32_t)m);
            a967_mul(B, L, (uint32_t)(m - c));
            a967_div(B, L, (uint32_t)c);
        }
    }
    uint64_t ip = (uint64_t)(F >> 64);
    uint64_t fp = (uint64_t)F;
    if (fp >= (1ULL << 63)) ip++;
    a967_addsmall(S, L, ip);

    seq_t val = 0;
    size_t i = L;
    while (i--) {
        uint32_t limb = S[i];
        for (int b = 3; b >= 0; b--) {
            val = val * 256;
            val = val + (unsigned)((limb >> (8 * b)) & 255u);
        }
    }
    free(B);
    free(T);
    free(S);
    return val;
}
