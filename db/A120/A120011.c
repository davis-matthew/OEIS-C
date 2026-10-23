/*
 * A120011  Decimal expansion of sqrt(3)/4.
 * https://oeis.org/A120011
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor( sqrt(3)*10^(n+1) / 4 ) mod 10 This computes the
 *            (n‑th) decimal digit of sqrt(3)/4
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define BASE 1000000000U   /* 1e9, fits in uint32_t */

typedef struct {
    size_t len;      /* number of used limbs (most significant limb is d[len-1]) */
    uint32_t *d;     /* limbs, little endian: d[0] is least significant */
} Big;

/* ---------- basic big integer helpers ---------- */
static void big_zero(Big *b) { b->len = 0; }

static void big_set_uint(Big *b, uint32_t v) {
    if (v == 0) { b->len = 0; return; }
    b->len = 1;
    b->d[0] = v;
}

static void big_copy(Big *dst, const Big *src) {
    dst->len = src->len;
    memcpy(dst->d, src->d, src->len * sizeof(uint32_t));
}

static int big_cmp(const Big *a, const Big *b) {
    if (a->len != b->len) return (a->len < b->len) ? -1 : 1;
    for (size_t i = a->len; i-- > 0;) {
        if (a->d[i] != b->d[i]) return (a->d[i] < b->d[i]) ? -1 : 1;
    }
    return 0;
}

/* multiply a by small integer m, store in res (may be same as a) */
static void big_mul_small(Big *res, const Big *a, uint32_t m) {
    if (m == 0 || a->len == 0) { res->len = 0; return; }
    uint64_t carry = 0;
    for (size_t i = 0; i < a->len; ++i) {
        uint64_t cur = (uint64_t)a->d[i] * m + carry;
        res->d[i] = (uint32_t)(cur % BASE);
        carry = cur / BASE;
    }
    res->len = a->len;
    while (carry > 0) {
        res->d[res->len++] = (uint32_t)(carry % BASE);
        carry /= BASE;
    }
}

/* add small integer addend to a (in place) */
static void big_add_small(Big *a, uint32_t addend) {
    if (addend == 0) return;
    uint64_t carry = addend;
    size_t i = 0;
    while (carry > 0 && i < a->len) {
        uint64_t cur = (uint64_t)a->d[i] + carry;
        a->d[i] = (uint32_t)(cur % BASE);
        carry = cur / BASE;
        ++i;
    }
    while (carry > 0) {
        a->d[a->len++] = (uint32_t)(carry % BASE);
        carry /= BASE;
    }
}

/* subtract b from a (a >= b), in place */
static void big_sub_big(Big *a, const Big *b) {
    int64_t borrow = 0;
    for (size_t i = 0; i < a->len; ++i) {
        int64_t sub = (int64_t)a->d[i] - (i < b->len ? b->d[i] : 0) - borrow;
        if (sub < 0) {
            sub += BASE;
            borrow = 1;
        } else borrow = 0;
        a->d[i] = (uint32_t)sub;
    }
    /* trim leading zeros */
    while (a->len > 0 && a->d[a->len - 1] == 0) --a->len;
}

/* divide a by small integer m, in place (quotient). */
static void big_div_small(Big *a, uint32_t m) {
    uint64_t rem = 0;
    for (size_t i = a->len; i-- > 0;) {
        uint64_t cur = rem * BASE + a->d[i];
        a->d[i] = (uint32_t)(cur / m);
        rem = cur % m;
    }
    /* trim leading zeros */
    while (a->len > 0 && a->d[a->len - 1] == 0) --a->len;
}

/* compute a mod 10, where a is in base BASE */
static uint32_t big_mod10(const Big *a) {
    uint64_t rem = 0;
    for (size_t i = a->len; i-- > 0;) {
        rem = (rem * BASE + a->d[i]) % 10;
    }
    return (uint32_t)rem;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    /* Number of digit groups in the integer N = 3 * 10^(2(n+1)) */
    idx_t groups = n + 2;

    /* Determine required limb size: at most (groups+1) decimal digits */
    size_t limbs = (groups + 8) / 9 + 2;   /* +2 safety */

    /* Allocate big integer arrays */
    Big P, R, tmp20, tmpK, cand;
    P.d = (uint32_t*)malloc(limbs * sizeof(uint32_t));
    R.d = (uint32_t*)malloc(limbs * sizeof(uint32_t));
    tmp20.d = (uint32_t*)malloc(limbs * sizeof(uint32_t));
    tmpK.d = (uint32_t*)malloc(limbs * sizeof(uint32_t));
    cand.d = (uint32_t*)malloc(limbs * sizeof(uint32_t));

    /* initialize */
    big_zero(&P);
    big_zero(&R);
    big_zero(&tmp20);
    big_zero(&tmpK);
    big_zero(&cand);

    for (idx_t g = 0; g < groups; ++g) {
        uint32_t G = (g == 0 ? 3U : 0U);

        /* R = R * 100 + G */
        big_mul_small(&R, &R, 100U);
        big_add_small(&R, G);

        /* tmp20 = P * 20 */
        big_mul_small(&tmp20, &P, 20U);

        int d;
        for (d = 9; d >= 0; --d) {
            /* tmpK = tmp20 + d */
            big_copy(&tmpK, &tmp20);
            big_add_small(&tmpK, (uint32_t)d);

            /* cand = tmpK * d */
            big_mul_small(&cand, &tmpK, (uint32_t)d);

            if (big_cmp(&cand, &R) <= 0)
                break;
        }

        /* R = R - cand */
        big_sub_big(&R, &cand);

        /* P = P * 10 + d */
        big_mul_small(&P, &P, 10U);
        big_add_small(&P, (uint32_t)d);
    }

    /* P now holds floor(sqrt(3) * 10^(n+1)) */
    /* compute floor(P / 4) */
    big_div_small(&P, 4U);

    /* obtain last decimal digit of the quotient */
    uint32_t digit = big_mod10(&P);

    /* free memory */
    free(P.d);
    free(R.d);
    free(tmp20.d);
    free(tmpK.d);
    free(cand.d);

    return (seq_t)digit;
}
