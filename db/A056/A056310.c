/*
 * A056310  Number of reversible strings with n beads using exactly three
 *          different colors.
 * https://oeis.org/A056310
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3^n + 3^ceil(n/2) - 3*(2^n + 2^ceil(n/2)) + 6) / 2, n ≥
 *            1
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>

/* Big integer in base 2^32 */
#define BIGINT_MAX_DIGITS 2000

typedef struct {
    uint32_t d[BIGINT_MAX_DIGITS];
    size_t len;          /* least‑significant digit first */
} BigInt;

/* initialise to a small value */
static inline void bi_init(BigInt *bi, uint32_t val) {
    bi->len = 1;
    bi->d[0] = val;
}

/* copy */
static inline void bi_copy(BigInt *dst, const BigInt *src) {
    dst->len = src->len;
    memcpy(dst->d, src->d, src->len * sizeof(uint32_t));
}

/* multiply by a small unsigned 32‑bit integer */
static inline void bi_mul_small(BigInt *bi, uint32_t m) {
    uint64_t carry = 0;
    for (size_t i = 0; i < bi->len; ++i) {
        uint64_t prod = (uint64_t)bi->d[i] * m + carry;
        bi->d[i] = (uint32_t)(prod & 0xFFFFFFFFU);
        carry   = prod >> 32;
    }
    if (carry) {
        bi->d[bi->len++] = (uint32_t)carry;
    }
}

/* add a small unsigned 32‑bit integer */
static inline void bi_add_small(BigInt *bi, uint32_t add) {
    uint64_t carry = add;
    for (size_t i = 0; i < bi->len; ++i) {
        if (carry == 0) break;
        uint64_t sum = (uint64_t)bi->d[i] + carry;
        bi->d[i] = (uint32_t)(sum & 0xFFFFFFFFU);
        carry   = sum >> 32;
    }
    if (carry) {
        bi->d[bi->len++] = (uint32_t)carry;
    }
}

/* add two BigInt numbers: a += b */
static inline void bi_add(BigInt *a, const BigInt *b) {
    size_t maxlen = (a->len > b->len) ? a->len : b->len;
    uint64_t carry = 0;
    for (size_t i = 0; i < maxlen; ++i) {
        uint64_t sum = carry;
        if (i < a->len) sum += a->d[i];
        if (i < b->len) sum += b->d[i];
        a->d[i] = (uint32_t)(sum & 0xFFFFFFFFU);
        carry   = sum >> 32;
    }
    if (carry) {
        a->d[maxlen] = (uint32_t)carry;
        a->len = maxlen + 1;
    } else {
        a->len = maxlen;
    }
}

/* subtract: a -= b  (assumes a ≥ b) */
static inline void bi_sub(BigInt *a, const BigInt *b) {
    uint64_t borrow = 0;
    for (size_t i = 0; i < a->len; ++i) {
        uint64_t sub = borrow;
        if (i < b->len) sub += b->d[i];
        if (a->d[i] >= sub) {
            a->d[i] = (uint32_t)(a->d[i] - sub);
            borrow = 0;
        } else {
            a->d[i] = (uint32_t)(a->d[i] + ((uint64_t)1 << 32) - sub);
            borrow = 1;
        }
    }
    while (a->len > 1 && a->d[a->len - 1] == 0) --a->len;
}

/* divide by 2 (shift right 1 bit) */
static inline void bi_shift_right1(BigInt *a) {
    uint32_t carry = 0;
    for (int i = (int)a->len - 1; i >= 0; --i) {
        uint64_t cur = ((uint64_t)carry << 32) | a->d[i];
        a->d[i] = (uint32_t)(cur >> 1);
        carry   = (uint32_t)(cur & 1);
    }
    while (a->len > 1 && a->d[a->len - 1] == 0) --a->len;
}

/* convert BigInt to seq_t (works for uint64_t and arbitrary precision types) */
static inline seq_t bigint_to_seq(const BigInt *bi) {
    seq_t res = 0;
    for (int i = (int)bi->len - 1; i >= 0; --i) {
        res = (res * (seq_t)(1ULL << 32)) + (seq_t)bi->d[i];
    }
    return res;
}

/* --------------------------------------------------------------- */

seq_t a(idx_t n) {
    /* powers of 3 and 2 for length n */
    BigInt pow3_n, pow2_n;
    bi_init(&pow3_n, 1);
    bi_init(&pow2_n, 1);
    for (idx_t i = 0; i < n; ++i) {
        bi_mul_small(&pow3_n, 3);
        bi_mul_small(&pow2_n, 2);
    }

    /* m = ceil(n/2) */
    idx_t m = (n + 1) / 2;

    /* powers of 3 and 2 for length m */
    BigInt pow3_m, pow2_m;
    bi_init(&pow3_m, 1);
    bi_init(&pow2_m, 1);
    for (idx_t i = 0; i < m; ++i) {
        bi_mul_small(&pow3_m, 3);
        bi_mul_small(&pow2_m, 2);
    }

    /* sum_n = 3^n + 3^m + 6 */
    BigInt sum_n;
    bi_copy(&sum_n, &pow3_n);
    bi_add(&sum_n, &pow3_m);
    bi_add_small(&sum_n, 6);

    /* sub_n = 3 * (2^n + 2^m) */
    BigInt sub_n;
    bi_copy(&sub_n, &pow2_n);
    bi_add(&sub_n, &pow2_m);
    bi_mul_small(&sub_n, 3);

    /* result = sum_n - sub_n */
    BigInt result;
    bi_copy(&result, &sum_n);
    bi_sub(&result, &sub_n);

    /* divide by 2 (reversible equivalence) */
    bi_shift_right1(&result);

    return bigint_to_seq(&result);
}
