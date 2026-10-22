/*
 * A002598  A generalized partition function.
 * https://oeis.org/A002598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f.: x*N(x)/((1-x)*(1-x^2)^2*(1-x^3)^3*(1-x^4)^4), where N(x) =
 *            1 +5x +x^2 -9x^3 -18x^4 -8x^5 +29x^6 +55x^7 +30x^8 -49x^9
 *            -119x^10 -81x^11 +50x^12 +166x^13 +157x^14 -11x^15 -172x^16
 *            -189x^17 -53x^18 +125x^19 +162x^20 +76x^21 -49x^22 -107x^23
 *            -54x^24 +9x^25 +43x^26 +29x^27 -2x^28 -10x^29 -9x^30 +...
 * Range:     u8 n <= 10, u16 n <= 30, u32 n <= 137, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#define A002598_GMAX 65536

static seq_t a002598_g[A002598_GMAX];
static idx_t a002598_glen = 0;

static const int a002598_num[33] = {
    1, 5, 1, -9, -18, -8, 29, 55, 30, -49, -119, -81, 50, 166, 157, -11,
    -172, -189, -53, 125, 162, 76, -49, -107, -54, 9, 43, 29, -2, -10, -9,
    0, 3
};

static void a002598_build(idx_t L)
{
    idx_t i;
    int k, r;
    for (i = 0; i < L; i++)
        a002598_g[i] = 0;
    a002598_g[0] = 1;
    for (k = 1; k <= 4; k++)
        for (r = 0; r < k; r++)
            for (i = (idx_t)k; i < L; i++)
                a002598_g[i] += a002598_g[i - (idx_t)k];
    a002598_glen = L;
}

seq_t a(idx_t n)
{
    seq_t pos = 0, neg = 0;
    idx_t i;
    if (n < 1)
        return 0;
    if (n > a002598_glen) {
        idx_t L = 64;
        while (L < n)
            L *= 2;
        if (L > A002598_GMAX)
            L = A002598_GMAX;
        a002598_build(L);
    }
    for (i = 0; i < 33 && i <= n - 1; i++) {
        idx_t j = n - 1 - i;
        int c = a002598_num[i];
        if (c > 0)
            pos += (seq_t)c * a002598_g[j];
        else if (c < 0)
            neg += (seq_t)(-c) * a002598_g[j];
    }
    return pos - neg;
}
