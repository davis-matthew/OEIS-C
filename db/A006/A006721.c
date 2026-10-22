/*
 * A006721  Somos-5 sequence: a(n) = (a(n-1) * a(n-4) + a(n-2) * a(n-3)) /
 *          a(n-5), with a(0) = a(1) = a(2) = a(3) = a(4) = 1.
 * https://oeis.org/A006721
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(n) = (a(n-1)*a(n-4) + a(n-2)*a(n-3)) / a(n-5), a(0) = a(1) =
 *            a(2) = a(3) = a(4) = 1
 * Range:     u8 n <= 10, u16 n <= 14, u32 n <= 19, u64 n <= 26, u128 n <= 37,
 *            big any n
 * Helpers:   alloc_zero, grow_zero, seq_is_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"
#include "../utils/seq_is_big.h"

/* Small fixed-size bignum, used for fixed-width builds so that the
   intermediate products (which overflow seq_t) are exact. */
#define NL 20
#define FIXED_MAX 48

typedef struct { uint32_t d[NL]; } bn;

static void bn_set(bn *r, uint32_t v) { memset(r, 0, sizeof(bn)); r->d[0] = v; }

static void bn_mul(const bn *x, const bn *y, bn *r) {
    uint32_t t[NL];
    memset(t, 0, sizeof(t));
    for (int i = 0; i < NL; i++) {
        if (x->d[i] == 0) continue;
        uint64_t carry = 0;
        for (int j = 0; i + j < NL; j++) {
            uint64_t cur = (uint64_t)x->d[i] * y->d[j] + t[i + j] + carry;
            t[i + j] = (uint32_t)cur;
            carry = cur >> 32;
        }
    }
    memcpy(r->d, t, sizeof(t));
}

static void bn_add(const bn *x, const bn *y, bn *r) {
    uint64_t carry = 0;
    for (int i = 0; i < NL; i++) {
        uint64_t cur = (uint64_t)x->d[i] + y->d[i] + carry;
        r->d[i] = (uint32_t)cur;
        carry = cur >> 32;
    }
}

static int bn_cmp(const bn *x, const bn *y) {
    for (int i = NL - 1; i >= 0; i--) {
        if (x->d[i] != y->d[i]) return x->d[i] < y->d[i] ? -1 : 1;
    }
    return 0;
}

static void bn_sub(bn *x, const bn *y) {
    int64_t borrow = 0;
    for (int i = 0; i < NL; i++) {
        int64_t cur = (int64_t)x->d[i] - y->d[i] - borrow;
        if (cur < 0) { cur += ((int64_t)1 << 32); borrow = 1; } else borrow = 0;
        x->d[i] = (uint32_t)cur;
    }
}

static void bn_shl1(bn *x) {
    for (int i = NL - 1; i > 0; i--) x->d[i] = (x->d[i] << 1) | (x->d[i - 1] >> 31);
    x->d[0] <<= 1;
}

static void bn_div(const bn *num, const bn *den, bn *q) {
    bn rem;
    bn_set(&rem, 0);
    bn_set(q, 0);
    for (int i = NL * 32 - 1; i >= 0; i--) {
        bn_shl1(&rem);
        rem.d[0] |= (num->d[i / 32] >> (i % 32)) & 1u;
        if (bn_cmp(&rem, den) >= 0) {
            bn_sub(&rem, den);
            q->d[i / 32] |= (uint32_t)1 << (i % 32);
        }
    }
}

static seq_t fixed_a(idx_t n) {
    static bn fv[FIXED_MAX + 1];
    static int ready = 0;
    if (!ready) {
        for (int i = 0; i <= 4; i++) bn_set(&fv[i], 1);
        for (int i = 5; i <= FIXED_MAX; i++) {
            bn p1, p2, s;
            bn_mul(&fv[i - 1], &fv[i - 4], &p1);
            bn_mul(&fv[i - 2], &fv[i - 3], &p2);
            bn_add(&p1, &p2, &s);
            bn_div(&s, &fv[i - 5], &fv[i]);
        }
        ready = 1;
    }
    if (n > FIXED_MAX) return 0; /* does not fit in any fixed-width seq_t */
    seq_t r = 0;
    for (int i = NL - 1; i >= 0; i--) {
        r = r * 65536;
        r = r * 65536;
        r += (idx_t)fv[n].d[i];
    }
    return r;
}

seq_t a(idx_t n) {
    if (!seq_is_big()) return fixed_a(n);

    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t computed = 0;

    if (cache == NULL) {
        cache_len = (n < 256) ? 256 : n + 1;
        cache = alloc_zero(cache_len);
        for (idx_t i = 0; i <= 4; i++) cache[i] = 1;
        computed = 5;
    }
    if (n >= cache_len) {
        idx_t new_len = n + n / 2 + 1;
        cache = grow_zero(cache, cache_len, new_len);
        cache_len = new_len;
    }
    while (computed <= n) {
        idx_t i = computed;
        cache[i] = (cache[i - 1] * cache[i - 4] + cache[i - 2] * cache[i - 3]) / cache[i - 5];
        computed++;
    }
    return cache[n];
}
