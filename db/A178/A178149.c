/*
 * A178149  Decimal expansion of (15+sqrt(1365))/30.
 * https://oeis.org/A178149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  cons, nonn
 * Formula:   a(n) = decimal digits of (15+sqrt(1365))/30
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          a(1) = 1 (integer part);
 *          a(2),a(3),... are the successive fractional digits of the number. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

/* ---------- bigint for base 1e9 ---------- */
#define BIGBASE 1000000000U
#define MAX_LIMBS 2000   /* enough for >15000 decimal digits */

typedef struct {
    uint32_t limb[MAX_LIMBS];
    size_t len;
} BigInt;

static void bigint_norm(BigInt *a) {
    while (a->len && a->limb[a->len-1] == 0) a->len--;
}

static void bigint_init(BigInt *a, uint32_t v) {
    if (v == 0) { a->len = 0; }
    else { a->len = 1; a->limb[0] = v; }
}

static void bigint_copy(BigInt *dst, const BigInt *src) {
    dst->len = src->len;
    memcpy(dst->limb, src->limb, src->len * sizeof(uint32_t));
}

static int bigint_cmp(const BigInt *a, const BigInt *b) {
    if (a->len != b->len) return (a->len > b->len) ? 1 : -1;
    for (int i = (int)a->len - 1; i >= 0; --i) {
        if (a->limb[i] != b->limb[i])
            return (a->limb[i] > b->limb[i]) ? 1 : -1;
    }
    return 0;
}

static void bigint_mul_small(BigInt *a, uint32_t m) {
    if (m == 0) { a->len = 0; return; }
    uint64_t carry = 0;
    for (size_t i = 0; i < a->len; ++i) {
        uint64_t prod = (uint64_t)a->limb[i] * m + carry;
        a->limb[i] = (uint32_t)(prod % BIGBASE);
        carry = prod / BIGBASE;
    }
    if (carry) {
        a->limb[a->len++] = (uint32_t)carry;
    }
}

static void bigint_add_small(BigInt *a, uint32_t v) {
    uint64_t carry = v;
    size_t i = 0;
    while (carry && i < a->len) {
        uint64_t sum = (uint64_t)a->limb[i] + carry;
        a->limb[i] = (uint32_t)(sum % BIGBASE);
        carry = sum / BIGBASE;
        ++i;
    }
    if (carry) a->limb[a->len++] = (uint32_t)carry;
}

static void bigint_sub(BigInt *a, const BigInt *b) {
    int64_t borrow = 0;
    size_t i = 0;
    for (i = 0; i < b->len; ++i) {
        int64_t diff = (int64_t)a->limb[i] - b->limb[i] - borrow;
        if (diff < 0) {
            diff += BIGBASE;
            borrow = 1;
        } else borrow = 0;
        a->limb[i] = (uint32_t)diff;
    }
    while (borrow && i < a->len) {
        int64_t diff = (int64_t)a->limb[i] - borrow;
        if (diff < 0) {
            diff += BIGBASE;
            borrow = 1;
        } else borrow = 0;
        a->limb[i] = (uint32_t)diff;
        ++i;
    }
    bigint_norm(a);
}

/* ---------- sequence computation ---------- */
#define MAX_DIGITS 20000   /* enough for n up to ~10 000 */
static bool initialized = false;
static BigInt root;           /* so far digits of sqrt(1365) */
static BigInt rem;            /* remainder in sqrt extraction */
static uint8_t seq_digits[MAX_DIGITS]; /* store fractional digits of y */
static size_t digits_len = 0;
static uint32_t div_rem = 0;  /* remainder for division by 30 */
static uint8_t carry_in = 0;  /* carry when adding 0.5 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0; /* out of range, return 0 */

    /* one‑time initialization */
    if (!initialized) {
        initialized = true;
        bigint_init(&root, 36U);          /* floor(sqrt(1365)) */
        bigint_init(&rem, 69U);           /* 1365 - 36^2 */
        div_rem = 36U % 30U;             /* remainder of integer part /30 */
        digits_len = 0;
        memset(seq_digits, 0, sizeof(seq_digits));
        carry_in = 0;
    }

    if (n == 1)   /* integer part of the number */
        return (seq_t)1;

    size_t need = (size_t)(n - 2);           /* index into seq_digits */
    if (need >= digits_len) {                /* generate more digits */
        for (size_t pos = digits_len; pos <= need; ++pos) {

            /* ------ sqrt extraction: generate next decimal digit of sqrt ------ */
            bigint_mul_small(&rem, 100U);            /* bring down two zeros */

            int d;
            BigInt cand;                            /* candidate value */
            for (d = 9; d >= 0; --d) {
                bigint_copy(&cand, &root);
                bigint_mul_small(&cand, 20U);
                bigint_add_small(&cand, (uint32_t)d);
                bigint_mul_small(&cand, (uint32_t)d);
                if (bigint_cmp(&rem, &cand) >= 0) break;
            }
            bigint_sub(&rem, &cand);                 /* update remainder */

            bigint_mul_small(&root, 10U);            /* shift root */
            bigint_add_small(&root, (uint32_t)d);    /* append new digit */

            /* ------ division by 30 of the growing sqrt number ------ */
            div_rem = div_rem * 10U + (uint32_t)d;   /* bring down the digit */
            uint32_t q_s = div_rem / 30U;
            div_rem %= 30U;

            /* ------ add 0.5 to produce the target number ------ */
            uint32_t val = q_s + carry_in;
            if (pos == 0)                  /* first fractional digit: add 5 */
                val += 5U;
            seq_digits[digits_len++] = (uint8_t)(val % 10U);
            carry_in = (uint8_t)(val / 10U);
        }
    }

    return (seq_t)seq_digits[need];
}
