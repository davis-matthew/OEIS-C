/*
 * A006402  Number of sensed 2-connected (nonseparable) planar maps with n
 *          edges.
 * https://oeis.org/A006402
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(2n)) * ( A000139(n-1) + Sum_{L|n, L>1}
 *            phi(L)*binomial(3n/L-1, n/L) + e(n) ), where A000139(n-1) =
 *            2*(3n-3)!/(n!*(2n-1)!), e(n) = 2*binomial(3k,k) if n = 2k+1 is
 *            odd, and e(n) = binomial(3k-2,k-1) if n = 2k is even
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 18, u64 n <= 31, u128 n <= 55,
 *            big any n
 * Helpers:   a006402_totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a006402_totient.h"

static void bn_mul(uint32_t *d, size_t *len, uint32_t m)
{
    uint64_t c = 0;
    size_t i;
    for (i = 0; i < *len; i++) {
        uint64_t t = (uint64_t)d[i] * m + c;
        d[i] = (uint32_t)t;
        c = t >> 32;
    }
    while (c) {
        d[(*len)++] = (uint32_t)c;
        c >>= 32;
    }
}

static void bn_div(uint32_t *d, size_t *len, uint32_t q)
{
    uint64_t r = 0;
    size_t i = *len;
    while (i-- > 0) {
        uint64_t cur = (r << 32) | d[i];
        d[i] = (uint32_t)(cur / q);
        r = cur % q;
    }
    while (*len > 1 && d[*len - 1] == 0) (*len)--;
}

static void bn_add(uint32_t *acc, size_t *alen, const uint32_t *b, size_t blen)
{
    uint64_t c = 0;
    size_t i;
    size_t mx = *alen > blen ? *alen : blen;
    for (i = *alen; i < mx; i++) acc[i] = 0;
    for (i = 0; i < mx; i++) {
        uint64_t t = (uint64_t)acc[i] + (i < blen ? b[i] : 0) + c;
        acc[i] = (uint32_t)t;
        c = t >> 32;
    }
    *alen = mx;
    if (c) acc[(*alen)++] = (uint32_t)c;
}

static void bn_binom(uint32_t *d, size_t *len, uint64_t N, uint64_t k)
{
    uint64_t i;
    d[0] = 1;
    *len = 1;
    for (i = 1; i <= k; i++) {
        bn_mul(d, len, (uint32_t)(N - k + i));
        bn_div(d, len, (uint32_t)i);
    }
}

seq_t a(idx_t n)
{
    size_t cap = (size_t)(3 * n) / 32 + 16;
    uint32_t *S = (uint32_t *)calloc(cap, sizeof(uint32_t));
    uint32_t *T = (uint32_t *)calloc(cap, sizeof(uint32_t));
    size_t sl = 1, tl = 1, i;
    idx_t L;
    seq_t res = 0;

    S[0] = 0;

    /* rooted nonseparable maps: 2*C(3n-3,n-1)/(n(2n-1)) */
    bn_binom(T, &tl, 3 * n - 3, n - 1);
    bn_mul(T, &tl, 2);
    bn_div(T, &tl, (uint32_t)n);
    bn_div(T, &tl, (uint32_t)(2 * n - 1));
    bn_add(S, &sl, T, tl);

    /* rotations of order L >= 2 with poles at vertices/faces */
    for (L = 2; L <= n; L++) {
        if (n % L == 0) {
            idx_t m = n / L;
            bn_binom(T, &tl, 3 * m - 1, m);
            bn_mul(T, &tl, a006402_totient((uint32_t)L));
            bn_add(S, &sl, T, tl);
        }
    }

    /* half-turns with pole(s) at edge midpoints */
    if (n % 2 == 1) {
        idx_t j = (n - 1) / 2;
        bn_binom(T, &tl, 3 * j, j);
        bn_mul(T, &tl, 2);
    } else {
        idx_t k = n / 2;
        bn_binom(T, &tl, 3 * k - 2, k - 1);
    }
    bn_add(S, &sl, T, tl);

    bn_div(S, &sl, (uint32_t)(2 * n));

    i = sl;
    while (i-- > 0) {
        int b;
        for (b = 3; b >= 0; b--) {
            uint32_t byte = (S[i] >> (8 * b)) & 255u;
            res *= 256;
            res += (seq_t)byte;
        }
    }

    free(S);
    free(T);
    return res;
}
