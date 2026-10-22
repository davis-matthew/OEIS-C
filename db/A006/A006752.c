/*
 * A006752  Decimal expansion of Catalan's constant 1 - 1/9 + 1/25 - 1/49 +
 *          1/81 - ...
 * https://oeis.org/A006752
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   G = (1/2)*Sum_{k>=0} (-1)^k * (3*k+2)*8^k /
 *            ((2*k+1)^3*C(2*k,k)^3); a(n) = floor(10^(n+1)*G) mod 10,
 *            evaluated in base-10^9 fixed point with native limbs (term ratio
 *            k^3/(2k-1)^3)
 * Range:     u8 n <= 9821, u16 n <= 9821, u32 n <= 9821, u64 n <= 9821,
 *            u128 n <= 9821, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define LB 1000000000ULL

static unsigned char *dig_cache = NULL;
static idx_t dig_count = 0;

/* x[s0..M-1] *= m, carry propagates to the left (into x[s0-1] etc.) */
static void fp_mul(uint32_t *x, size_t s, size_t M, uint64_t m) {
    uint64_t carry = 0;
    size_t i = M;
    while (i > s) {
        i--;
        uint64_t cur = (uint64_t)x[i] * m + carry;
        x[i] = (uint32_t)(cur % LB);
        carry = cur / LB;
    }
    while (carry && i > 0) {
        i--;
        uint64_t cur = (uint64_t)x[i] + carry;
        x[i] = (uint32_t)(cur % LB);
        carry = cur / LB;
    }
}

static void fp_div(uint32_t *x, size_t s, size_t M, uint64_t d) {
    uint64_t rem = 0;
    size_t i;
    for (i = s; i < M; i++) {
        uint64_t cur = rem * LB + x[i];
        x[i] = (uint32_t)(cur / d);
        rem = cur % d;
    }
}

static void fp_add(uint32_t *acc, const uint32_t *t, size_t s, size_t M) {
    uint64_t carry = 0;
    size_t i = M;
    while (i > s) {
        i--;
        uint64_t cur = (uint64_t)acc[i] + t[i] + carry;
        acc[i] = (uint32_t)(cur % LB);
        carry = cur / LB;
    }
    while (carry && i > 0) {
        i--;
        uint64_t cur = (uint64_t)acc[i] + carry;
        acc[i] = (uint32_t)(cur % LB);
        carry = cur / LB;
    }
}

static void compute_digits(idx_t D) {
    size_t M = 1 + (size_t)((D + 8) / 9) + 3;
    uint32_t *v = (uint32_t *)calloc(M, sizeof(uint32_t));
    uint32_t *t = (uint32_t *)calloc(M, sizeof(uint32_t));
    uint32_t *sp = (uint32_t *)calloc(M, sizeof(uint32_t));
    uint32_t *sm = (uint32_t *)calloc(M, sizeof(uint32_t));
    size_t start = 0, i;
    uint64_t k;
    int r;

    v[0] = 1; /* v_k = 8^k / C(2k,k)^3 */
    for (k = 0;; k++) {
        if (k > 0) {
            size_t s0 = start > 0 ? start - 1 : 0;
            for (r = 0; r < 3; r++) {
                fp_mul(v, start, M, k);
                fp_div(v, s0, M, 2 * k - 1);
            }
            while (start < M && v[start] == 0) start++;
            /* the multiply may have pushed into s0; recheck from s0 */
            if (s0 < start && v[s0] != 0) start = s0;
        }
        if (start >= M) break;
        {
            size_t s0 = start > 0 ? start - 1 : 0;
            for (i = s0; i < M; i++) t[i] = (i >= start) ? v[i] : 0;
            fp_mul(t, start, M, 3 * k + 2);
            for (r = 0; r < 3; r++) fp_div(t, s0, M, 2 * k + 1);
            fp_add((k % 2 == 0) ? sp : sm, t, s0, M);
        }
    }

    /* sp -= sm */
    {
        int64_t borrow = 0;
        i = M;
        while (i > 0) {
            i--;
            int64_t cur = (int64_t)sp[i] - (int64_t)sm[i] - borrow;
            if (cur < 0) { cur += (int64_t)LB; borrow = 1; } else borrow = 0;
            sp[i] = (uint32_t)cur;
        }
    }
    fp_div(sp, 0, M, 2);

    unsigned char *nd = (unsigned char *)malloc((size_t)D);
    for (idx_t n = 0; n < D; n++) {
        uint32_t limb = sp[1 + n / 9];
        int pos = (int)(n % 9);
        uint32_t p = 1;
        for (r = 0; r < 8 - pos; r++) p *= 10;
        nd[n] = (unsigned char)((limb / p) % 10);
    }
    free(v); free(t); free(sp); free(sm);
    if (dig_cache) free(dig_cache);
    dig_cache = nd;
    dig_count = D;
}

seq_t a(idx_t n) {
    if (dig_cache == NULL || n >= dig_count) {
        idx_t target = n + 1;
        idx_t grow = dig_count + dig_count / 2;
        if (target < grow) target = grow;
        target += 32;
        compute_digits(target);
    }
    return (seq_t)dig_cache[n];
}
