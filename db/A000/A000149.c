/*
 * A000149  a(n) = floor(e^n).
 * https://oeis.org/A000149
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(e^n), with e^n = 1 + n/1*(1 + n/2*(1 + n/3*(1 + ...
 *            (1 + n/K)))), evaluated by Horner's rule in multi-limb fixed
 *            point and truncated to its integer part
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 44, u128 n <= 88,
 *            big any n
 * Validated: standard, pass, 45 terms through n = 293 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Computes floor(e^n) into seq_t using G guard limbs; returns 0 if the
   result is too close to an integer boundary to be certain, 1 otherwise. */
static int a149_try(idx_t n, unsigned G, seq_t *out)
{
    uint64_t I = (uint64_t)n * 14427u / 320000u + 3; /* integer limbs */
    uint64_t F = I + G;                              /* fractional limbs */
    uint64_t L = I + F;
    uint64_t K = 3 * (uint64_t)n + 600;              /* series terms */
    uint32_t *v = (uint32_t *)calloc((size_t)L, sizeof(uint32_t));
    uint64_t hi, k, i;
    int ok;
    seq_t r;

    v[F] = 1;           /* v = 1 */
    hi = F;             /* index of highest possibly-nonzero limb */
    for (k = K; k >= 1; k--) {
        uint64_t carry = 0, rem = 0;
        /* v *= n */
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
        /* v /= k */
        for (i = hi + 1; i-- > 0;) {
            uint64_t cur = (rem << 32) | v[i];
            v[i] = (uint32_t)(cur / k);
            rem = cur % k;
        }
        while (hi > F && v[hi] == 0) hi--;
        /* v += 1 */
        for (i = F; i < L; i++) {
            v[i]++;
            if (v[i] != 0) break;
        }
        if (i > hi && i < L) hi = i;
    }

    /* truncation errors are all downward and below 2^-64; a fractional
       part this close to 1 would be ambiguous */
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

seq_t a(idx_t n)
{
    unsigned G = 2;
    seq_t r = 0;
    while (!a149_try(n, G, &r))
        G += 4;
    return r;
}
