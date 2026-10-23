/*
 * A177272  Decimal expansion of sqrt(193).
 * https://oeis.org/A177272
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  cons, nonn
 * Formula:   not stated
 * Range:     u8 n <= 106, u16 n <= 106, u32 n <= 106, u64 n <= 106,
 *            u128 n <= 106, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th digit of the decimal expansion of sqrt(193)
   (offset 2: a(2)=1, a(3)=3, a(4)=8, …).  */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- Big integer (base 1 000 000 000) ---------- */
#define BI_BASE 1000000000U

typedef struct {
    size_t len;      /* number of limbs used (at least 1) */
    size_t cap;      /* allocated number of limbs        */
    uint32_t *l;     /* little‑endian limb array          */
} BigInt;

static void bi_expand(BigInt *a, size_t need) {
    if (a->cap >= need) return;
    size_t new_cap = a->cap ? a->cap * 2 : 16;
    if (new_cap < need) new_cap = need;
    uint32_t *tmp = (uint32_t *)realloc(a->l, new_cap * sizeof(uint32_t));
    if (!tmp) exit(1);
    a->l = tmp;
    a->cap = new_cap;
}

static void bi_init(BigInt *a) { a->len = 0; a->cap = 0; a->l = NULL; }

static void bi_free(BigInt *a) { if (a->l) free(a->l); a->len = 0; a->cap = 0; a->l = NULL; }

static void bi_set_uint(BigInt *a, uint64_t v) {
    bi_free(a);
    if (v == 0) {
        a->len = 1;
        bi_expand(a, 1);
        a->l[0] = 0;
        return;
    }
    a->len = 0;
    while (v) {
        bi_expand(a, a->len + 1);
        a->l[a->len++] = (uint32_t)(v % BI_BASE);
        v /= BI_BASE;
    }
}

static void bi_copy(const BigInt *src, BigInt *dst) {
    bi_expand(dst, src->len);
    dst->len = src->len;
    for (size_t i = 0; i < src->len; ++i) dst->l[i] = src->l[i];
}

static void bi_mul_small(BigInt *a, uint32_t m) {
    if (m == 0) { a->len = 1; a->l[0] = 0; return; }
    uint64_t carry = 0;
    for (size_t i = 0; i < a->len; ++i) {
        uint64_t prod = (uint64_t)a->l[i] * m + carry;
        a->l[i] = (uint32_t)(prod % BI_BASE);
        carry = prod / BI_BASE;
    }
    while (carry) {
        bi_expand(a, a->len + 1);
        a->l[a->len++] = (uint32_t)(carry % BI_BASE);
        carry /= BI_BASE;
    }
}

static void bi_add_small(BigInt *a, uint32_t addend) {
    uint64_t carry = addend;
    size_t i = 0;
    while (carry) {
        if (i >= a->len) {
            bi_expand(a, a->len + 1);
            a->l[a->len++] = 0;
        }
        uint64_t sum = (uint64_t)a->l[i] + carry;
        a->l[i] = (uint32_t)(sum % BI_BASE);
        carry = sum / BI_BASE;
        ++i;
    }
}

static int bi_cmp(const BigInt *a, const BigInt *b) {
    if (a->len != b->len) return (a->len > b->len) ? 1 : -1;
    for (size_t i = a->len; i-- > 0;) {
        if (a->l[i] != b->l[i])
            return (a->l[i] > b->l[i]) ? 1 : -1;
    }
    return 0;
}

static void bi_subtract(BigInt *a, const BigInt *b) {
    /* assume a >= b */
    int64_t borrow = 0;
    for (size_t i = 0; i < a->len; ++i) {
        int64_t ai = a->l[i];
        int64_t bi = (i < b->len) ? b->l[i] : 0;
        int64_t diff = ai - bi - borrow;
        if (diff < 0) {
            diff += BI_BASE;
            borrow = 1;
        } else {
            borrow = 0;
        }
        a->l[i] = (uint32_t)diff;
    }
    while (a->len > 1 && a->l[a->len - 1] == 0) a->len--;
}

/* ---------- Digit cache ---------- */
static uint8_t *digit_cache = NULL;   /* array of digits (0-9) */
static size_t digit_cache_len = 0;    /* number of digits stored */
static size_t digit_cache_cap = 0;    /* capacity of digit_cache */

static void digit_cache_expand(size_t needed) {
    if (digit_cache_cap >= needed) return;
    size_t new_cap = digit_cache_cap ? digit_cache_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    uint8_t *tmp = (uint8_t *)realloc(digit_cache, new_cap * sizeof(uint8_t));
    if (!tmp) exit(1);
    digit_cache = tmp;
    digit_cache_cap = new_cap;
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n) {
    if (n < 2) return (seq_t)0;  /* undefined below offset */

    /* target index in the digit cache (0‑based).  a(2) -> cache[0] */
    size_t target = n - 2;

    static int init_done = 0;
    static BigInt big_p, big_r;

    if (!init_done) {
        /* integer part of sqrt(193) is 13 */
        bi_init(&big_p);
        bi_set_uint(&big_p, 13U);

        /* remainder = 193 - 13^2 = 24 */
        bi_init(&big_r);
        bi_set_uint(&big_r, 24U);

        /* cache first two digits of integer part: 1, 3 */
        digit_cache_expand(2);
        digit_cache[0] = 1U;
        digit_cache[1] = 3U;
        digit_cache_len = 2;

        init_done = 1;
    }

    /* generate digits up to required position */
    while (digit_cache_len <= target) {
        /* r = r * 100 */
        bi_mul_small(&big_r, 100U);

        uint32_t chosen_d = 0;
        BigInt temp, prod, best;
        bi_init(&temp);
        bi_init(&prod);
        bi_init(&best);

        /* try digits from 9 down to 0 */
        for (int d = 9; d >= 0; --d) {
            /* temp = 20 * p */
            bi_copy(&big_p, &temp);
            bi_mul_small(&temp, 20U);

            /* temp = temp + d */
            bi_add_small(&temp, (uint32_t)d);

            /* prod = temp * d */
            bi_copy(&temp, &prod);
            bi_mul_small(&prod, (uint32_t)d);

            if (bi_cmp(&prod, &big_r) <= 0) {
                chosen_d = (uint32_t)d;
                bi_copy(&prod, &best);
                break;
            }
        }

        /* append chosen digit */
        digit_cache_expand(digit_cache_len + 1);
        digit_cache[digit_cache_len++] = (uint8_t)chosen_d;

        /* r = r - best */
        bi_subtract(&big_r, &best);

        /* p = p * 10 + chosen_d */
        bi_mul_small(&big_p, 10U);
        bi_add_small(&big_p, chosen_d);

        bi_free(&temp);
        bi_free(&prod);
        bi_free(&best);
    }

    return (seq_t)digit_cache[target];
}
