/*
 * A003093  Number of planar 2-trees with n nodes.
 * https://oeis.org/A003093
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = c(n) + [n even]*(1/2)*C5(n/2-1) + [3|n]*(2/3)*D5(n/3-1),
 *            where c(n) = (n-1)*binomial(5m,m)/((4m+1)*(4m+2)) with m = n-2,
 *            C5(j) = binomial(5j,j)/(4j+1) are the coefficients of B(x), B =
 *            1 + x*B^5, and D5(j) = 2*binomial(5j+2,j)/(5j+2) are the
 *            coefficients of B(x)^2. This comes from the OEIS...
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 22, u128 n <= 23,
 *            big any n
 * Validated: partial, pass, 21 terms through n = 23 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void p3093_addto(uint32_t *x, const uint32_t *y, size_t L)
{
    uint64_t c = 0;
    size_t i;
    for (i = 0; i < L; i++) {
        uint64_t t = (uint64_t)x[i] + y[i] + c;
        x[i] = (uint32_t)t;
        c = t >> 32;
    }
}

static void p3093_div(uint32_t *x, size_t L, uint32_t d)
{
    uint64_t r = 0;
    size_t i = L;
    while (i-- > 0) {
        uint64_t cur = (r << 32) | x[i];
        x[i] = (uint32_t)(cur / d);
        r = cur % d;
    }
}

static void p3093_mul(uint32_t *x, size_t L, uint32_t k)
{
    uint64_t c = 0;
    size_t i;
    for (i = 0; i < L; i++) {
        uint64_t t = (uint64_t)x[i] * k + c;
        x[i] = (uint32_t)t;
        c = t >> 32;
    }
}

/* x = binomial(N, K), computed multiplicatively (exact at every step) */
static void p3093_binom(uint32_t *x, size_t L, uint64_t N, uint64_t K)
{
    uint64_t i;
    memset(x, 0, L * sizeof(uint32_t));
    x[0] = 1;
    for (i = 1; i <= K; i++) {
        p3093_mul(x, L, (uint32_t)(N - K + i));
        p3093_div(x, L, (uint32_t)i);
    }
}

seq_t a(idx_t n)
{
    size_t L = (size_t)((6 * n + 160) / 32 + 4);
    uint32_t *S = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *t = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint64_t m = n - 2;
    seq_t r = 0;
    size_t top, i;
    int b;

    /* 6*c(n) */
    p3093_binom(S, L, 5 * m, m);
    p3093_mul(S, L, 6);
    p3093_mul(S, L, (uint32_t)(n - 1));
    p3093_div(S, L, (uint32_t)(4 * m + 1));
    p3093_div(S, L, (uint32_t)(4 * m + 2));

    if (n % 2 == 0) {
        uint64_t j = n / 2 - 1;
        p3093_binom(t, L, 5 * j, j);
        p3093_div(t, L, (uint32_t)(4 * j + 1));
        p3093_mul(t, L, 3);
        p3093_addto(S, t, L);
    }
    if (n % 3 == 0) {
        uint64_t j = n / 3 - 1;
        p3093_binom(t, L, 5 * j + 2, j);
        p3093_mul(t, L, 8);
        p3093_div(t, L, (uint32_t)(5 * j + 2));
        p3093_addto(S, t, L);
    }
    p3093_div(S, L, 6);

    top = L;
    while (top > 0 && S[top - 1] == 0) top--;
    for (i = top; i-- > 0;) {
        for (b = 3; b >= 0; b--) {
            idx_t byte = (idx_t)((S[i] >> (8 * b)) & 255u);
            r = r * 256 + byte;
        }
    }
    free(S);
    free(t);
    return r;
}
