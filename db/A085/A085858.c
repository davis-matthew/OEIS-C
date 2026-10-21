/*
 * A085858  Number of 4's in decimal expansion of Fibonacci(n).
 * https://oeis.org/A085858
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = number of decimal occurrences of the digit 4 in
 *            Fibonacci(n) (F(0) = 0, F(1) = 1, F(k) = F(k-1)+F(k-2) for k ≥
 *            2)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <string.h>
 
/* Big integer in base 1 000 000 000 (1e9) with a fixed maximum size. */
#define BIG_MAX_LIMBS 256            /* enough for n ≤ 100 000 (≈20 000 digits) */
#define BIG_BASE 1000000000U
 
typedef struct {
    uint32_t d[BIG_MAX_LIMBS];   /* little‑endian limbs (least significant first) */
    size_t   len;                /* number of used limbs (len == 0 ⇔ value == 0) */
} BigInt;
 
static void big_set_uint(BigInt *x, uint32_t v) {
    if (v == 0) {
        x->len = 0;
    } else {
        x->len = 1;
        x->d[0] = v;
    }
}
 
static void big_copy(BigInt *dst, const BigInt *src) {
    dst->len = src->len;
    memcpy(dst->d, src->d, src->len * sizeof(uint32_t));
}
 
/* x + y → res (all three arguments may alias only between src1 and src2) */
static void big_add(const BigInt *x, const BigInt *y, BigInt *res) {
    const uint32_t *ax = x->d;
    const uint32_t *by = y->d;
    uint32_t *rx = res->d;
    size_t i, max = (x->len > y->len) ? x->len : y->len;
    uint64_t carry = 0;
    for (i = 0; i < max; ++i) {
        uint64_t av = (i < x->len) ? ax[i] : 0U;
        uint64_t bv = (i < y->len) ? by[i] : 0U;
        uint64_t sum = av + bv + carry;
        rx[i] = (uint32_t)(sum % BIG_BASE);
        carry  = sum / BIG_BASE;
    }
    if (carry) {
        rx[i++] = (uint32_t)carry;
    }
    res->len = i;
}
 
/* Count decimal digit '4' in a big integer. */
static uint64_t count_fours_in_big(const BigInt *x) {
    if (x->len == 0) return 0;
    uint64_t cnt = 0;
    size_t i;
    /* Process the most significant limb first (no padding). */
    uint32_t val = x->d[x->len - 1];
    if (val == 0) {
        /* shouldn't happen except for x == 0 which is handled above */
    } else {
        while (val) {
            if ((val % 10) == 4U) ++cnt;
            val /= 10U;
        }
    }
    /* Process remaining limbs (each is exactly 9 decimal digits with leading zeros). */
    for (i = x->len - 2; i < x->len; --i) {   /* i is signed, stop after i==0 */
        uint32_t limb = x->d[i];
        /* Each limb contributes exactly 9 digits, even if some are leading zeros. */
        size_t j;
        for (j = 0; j < 9; ++j) {
            if ((limb % 10) == 4U) ++cnt;
            limb /= 10U;
        }
    }
    return cnt;
}
 
seq_t a(idx_t n) {
    /* Compute F(n) iteratively using a simple addition loop. */
    BigInt a_val, b_val, tmp;
    big_set_uint(&a_val, 0);   /* F(0) */
    big_set_uint(&b_val, 1);   /* F(1) */
    for (idx_t i = 0; i < n; ++i) {
        big_add(&a_val, &b_val, &tmp);  /* tmp = a_val + b_val   */
        big_copy(&a_val, &b_val);      /* a_val = b_val         */
        big_copy(&b_val, &tmp);        /* b_val = tmp           */
    }
    /* Now a_val holds F(n). Count the digit '4'. */
    uint64_t cnt = count_fours_in_big(&a_val);
    return (seq_t)cnt;
}
