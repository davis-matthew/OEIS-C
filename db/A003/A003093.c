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
 * Helpers:   p3093_addto, p3093_binom, p3093_div, p3093_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/p3093_addto.h"
#include "../utils/p3093_binom.h"
#include "../utils/p3093_div.h"
#include "../utils/p3093_mul.h"

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
