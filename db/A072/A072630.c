/*
 * A072630  Values of n where A072629 switches from 01010.. into 0000.. or
 *          back.
 * https://oeis.org/A072630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000149(n) if that is odd, else A000149(n)-1, where
 *            A000149(n) = floor(e^n)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Validated: partial, pass, 26 terms through n = 26 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int A000149_try(idx_t n, unsigned G, seq_t *out)
{
    uint64_t I = (uint64_t)n * 14427u / 320000u + 3;
    uint64_t F = I + G;
    uint64_t L = I + F;
    uint64_t K = 3 * (uint64_t)n + 600;
    uint32_t *v = (uint32_t *)calloc((size_t)L, sizeof(uint32_t));
    uint64_t hi, k, i;
    int ok;
    seq_t r;

    v[F] = 1;
    hi = F;
    for (k = K; k >= 1; k--) {
        uint64_t carry = 0, rem = 0;
        for (i = 0; i <= hi; i++) {
            uint64_t t = (uint64_t)v[i] * (uint64_t)n + carry;
            v[i] = (uint32_t)t;
            carry = t >> 32;
        }
        while (carry && hi + 1 < L) {
            hi++;
            v[hi] = (uint32_t)carry;
            carry >>= 32;
        }
        for (i = hi + 1; i-- > 0;) {
            uint64_t cur = (rem << 32) | v[i];
            v[i] = (uint32_t)(cur / k);
            rem = cur % k;
        }
        while (hi > F && v[hi] == 0) hi--;
        for (i = F; i < L; i++) {
            v[i]++;
            if (v[i] != 0) break;
        }
        if (i > hi && i < L) hi = i;
    }

    ok = !(v[F - 1] == 0xFFFFFFFFu && v[F - 2] == 0xFFFFFFFFu);

    r = 0;
    for (i = L; i-- > F;) {
        int b;
        for (b = 3; b >= 0; b--) {
            r = r * 256;
            r += (v[i] >> (8 * b)) & 255u;
        }
    }
    free(v);
    *out = r;
    return ok;
}

static seq_t A000149_a(idx_t n)
{
    unsigned G = 2;
    seq_t r = 0;
    while (!A000149_try(n, G, &r))
        G += 4;
    return r;
}

seq_t a(idx_t n)
{
    seq_t r = A000149_a(n);
    if (r % 2 == 1) return r;
    return r - 1;
}
