/*
 * A004567  Expansion of sqrt(6) in base 8.
 * https://oeis.org/A004567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, cons, easy
 * Formula:   a(1) = floor(sqrt(6)) = 2; a(n) = floor(sqrt(6)*8^(n-1)) mod 8,
 *            with the bits of sqrt(6) produced by exact binary digit-by-digit
 *            square root on multi-limb integers (3 bits per octal digit)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static unsigned char *a4567_dig = NULL;
static idx_t a4567_ndig = 0;

static void a4567_compute(idx_t N) {
    idx_t nb = 3 * (N - 1);            /* fractional bits needed */
    idx_t L = (nb + 8) / 32 + 3;       /* limbs */
    uint32_t *p = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *r = (uint32_t *)calloc(L, sizeof(uint32_t));
    unsigned char *dig = (unsigned char *)calloc(N + 1, 1);
    idx_t total = 2 + nb, step, i;
    idx_t di = 1;
    unsigned cur = 0, cnt = 0;

    for (step = 0; step < total; step++) {
        uint32_t pair = (step == 0) ? 1u : (step == 1 ? 2u : 0u);
        idx_t up = (step + 8) / 32 + 2;
        uint32_t carry, bit;
        int ge;
        if (up > L) up = L;

        /* r = (r << 2) | pair */
        carry = pair;
        for (i = 0; i < up; i++) {
            uint32_t nc = r[i] >> 30;
            r[i] = (r[i] << 2) | carry;
            carry = nc;
        }

        /* compare r with t = (p << 2) | 1 */
        ge = 1;
        for (i = up; i-- > 0;) {
            uint32_t t = (p[i] << 2) | (i > 0 ? (p[i - 1] >> 30) : 0u) | (i == 0 ? 1u : 0u);
            if (r[i] != t) { ge = (r[i] > t); break; }
        }

        if (ge) {
            uint64_t borrow = 0;
            for (i = 0; i < up; i++) {
                uint32_t t = (p[i] << 2) | (i > 0 ? (p[i - 1] >> 30) : 0u) | (i == 0 ? 1u : 0u);
                uint64_t d = (uint64_t)r[i] - t - borrow;
                r[i] = (uint32_t)d;
                borrow = (d >> 63) & 1u;
            }
            bit = 1;
        } else {
            bit = 0;
        }

        /* p = (p << 1) | bit */
        carry = bit;
        for (i = 0; i < up; i++) {
            uint32_t nc = p[i] >> 31;
            p[i] = (p[i] << 1) | carry;
            carry = nc;
        }

        if (step == 1) {
            dig[0] = (unsigned char)p[0];   /* integer part = 2 */
        } else if (step >= 2) {
            cur = (cur << 1) | bit;
            if (++cnt == 3) {
                dig[di++] = (unsigned char)cur;
                cur = 0;
                cnt = 0;
            }
        }
    }

    free(p);
    free(r);
    free(a4567_dig);
    a4567_dig = dig;
    a4567_ndig = N;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (n > a4567_ndig) {
        idx_t N = n;
        if (N < 2 * a4567_ndig) N = 2 * a4567_ndig;
        if (N < 64) N = 64;
        a4567_compute(N);
    }
    return (seq_t)a4567_dig[n - 1];
}
