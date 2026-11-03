/*
 * A003449  Number of nonequivalent dissections of an n-gon into n-3 polygons
 *          by nonintersecting diagonals up to rotation and reflection.
 * https://oeis.org/A003449
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   Burnside over the dihedral group D_n, with C(k) = Catalan(k) and
 *            K3(N) = 3*binomial(2N+3,N)/(2N+3) = [x^N] C(x)^3: n odd,
 *            m=(n-1)/2: a(n) = ( binomial(2n-5,n-4) + n*C(m-1) ) / (2n) n
 *            even, m=n/2: a(n) = ( binomial(2n-5,n-4) + m*C(m-1)/2 +
 *            m*(C(m-1) + K3(m-3)) + m*C(m-2) + [4|n] * 2*(n/4)*C(n/4-1)...
 * Range:     u8 n <= 9, u16 n <= 14, u32 n <= 22, u64 n <= 39, u128 n <= 72,
 *            big any n
 * Validated: standard, pass, 43 terms through n = 199 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void a3449_add(uint32_t *acc, const uint32_t *x, size_t L)
{
    uint64_t c = 0;
    size_t i;
    for (i = 0; i < L; i++) {
        c += (uint64_t)acc[i] + x[i];
        acc[i] = (uint32_t)c;
        c >>= 32;
    }
}

static void a3449_div(uint32_t *x, size_t L, uint32_t d)
{
    uint64_t r = 0;
    size_t i = L;
    while (i--) {
        uint64_t cur = (r << 32) | x[i];
        x[i] = (uint32_t)(cur / d);
        r = cur % d;
    }
}

static void a3449_mul(uint32_t *x, size_t L, uint32_t m)
{
    uint64_t c = 0;
    size_t i;
    for (i = 0; i < L; i++) {
        c += (uint64_t)x[i] * m;
        x[i] = (uint32_t)c;
        c >>= 32;
    }
}

static void a3449_binom(uint32_t *x, size_t L, uint64_t N, uint64_t k)
{
    uint64_t i;
    memset(x, 0, L * sizeof(uint32_t));
    x[0] = 1;
    for (i = 1; i <= k; i++) {
        a3449_mul(x, L, (uint32_t)(N - k + i));
        a3449_div(x, L, (uint32_t)i);
    }
}

static void a3449_cat(uint32_t *x, size_t L, uint64_t k)
{
    a3449_binom(x, L, 2 * k, k);
    a3449_div(x, L, (uint32_t)(k + 1));
}

static void a3449_k3(uint32_t *x, size_t L, uint64_t N)
{
    a3449_binom(x, L, 2 * N + 3, N);
    a3449_mul(x, L, 3);
    a3449_div(x, L, (uint32_t)(2 * N + 3));
}

seq_t a(idx_t n)
{
    size_t L = (size_t)((4 * n + 192) / 32 + 2);
    uint32_t *acc = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *tmp = (uint32_t *)calloc(L, sizeof(uint32_t));
    seq_t r = 0;
    size_t top, i;
    int b;

    /* accumulate 2*S, divide by 4n at the end */
    a3449_binom(tmp, L, 2 * n - 5, n - 4);
    a3449_mul(tmp, L, 2);
    a3449_add(acc, tmp, L);

    if (n % 2 == 1) {
        uint64_t m = (n - 1) / 2;
        a3449_cat(tmp, L, m - 1);
        a3449_mul(tmp, L, (uint32_t)(2 * n));
        a3449_add(acc, tmp, L);
    } else {
        uint64_t m = n / 2;
        /* rotation by half-turn (m*C(m-1)/2, doubled) + vertex axes C(m-1) part */
        a3449_cat(tmp, L, m - 1);
        a3449_mul(tmp, L, (uint32_t)(3 * m));
        a3449_add(acc, tmp, L);
        if (m >= 3) {
            a3449_k3(tmp, L, m - 3);
            a3449_mul(tmp, L, (uint32_t)(2 * m));
            a3449_add(acc, tmp, L);
        }
        a3449_cat(tmp, L, m - 2);
        a3449_mul(tmp, L, (uint32_t)(2 * m));
        a3449_add(acc, tmp, L);
        if (n % 4 == 0) {
            uint64_t q = n / 4;
            a3449_cat(tmp, L, q - 1);
            a3449_mul(tmp, L, (uint32_t)(4 * q));
            a3449_add(acc, tmp, L);
        }
    }

    a3449_div(acc, L, (uint32_t)(4 * n));

    top = L;
    while (top > 0 && acc[top - 1] == 0) top--;
    for (i = top; i-- > 0;) {
        for (b = 3; b >= 0; b--) {
            r = r * 256;
            r += (seq_t)((acc[i] >> (8 * b)) & 0xFFu);
        }
    }

    free(acc);
    free(tmp);
    return r;
}
