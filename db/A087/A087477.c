/*
 * A087477  Decimal expansion of sqrt(51)-4.
 * https://oeis.org/A087477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, cons
 * Formula:   a(n) = nth decimal digit of sqrt(51)-4
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define BASE 1000000000U   /* 1e9 */
#define MAX_D 3000         /* enough for 20000 decimal digits */

/* ------------------------------------------------------------------- */
/* Small arbitrary‑precision unsigned integer (biguint)                 */
/* ------------------------------------------------------------------- */
typedef struct {
    uint32_t d[MAX_D];
    size_t len;          /* number of used limbs (little endian) */
} BigUint;

/* initialize to zero */
static inline void big_zero(BigUint *a) {
    a->len = 0;
}

/* trim leading zero limbs */
static inline void big_trim(BigUint *a) {
    while (a->len && a->d[a->len-1] == 0) a->len--;
}

/* copy from src to dst */
static inline void big_copy(BigUint *dst, const BigUint *src) {
    dst->len = src->len;
    memcpy(dst->d, src->d, src->len * sizeof(uint32_t));
}

/* compare a and b: return -1 if a<b, 0 if a==b, 1 if a>b */
static inline int big_cmp(const BigUint *a, const BigUint *b) {
    if (a->len != b->len) return (a->len < b->len) ? -1 : 1;
    for (size_t i = a->len; i>0; --i) {
        uint32_t ai = a->d[i-1];
        uint32_t bi = b->d[i-1];
        if (ai != bi) return (ai < bi) ? -1 : 1;
    }
    return 0;
}

/* a += small (0 <= v < BASE) */
static inline void big_add_small(BigUint *a, uint32_t v) {
    uint64_t carry = v;
    size_t i = 0;
    while (carry && i < a->len) {
        uint64_t sum = (uint64_t)a->d[i] + carry;
        a->d[i] = (uint32_t)(sum % BASE);
        carry = sum / BASE;
        ++i;
    }
    if (carry) {
        a->d[a->len++] = (uint32_t)carry;
    }
}

/* a *= small (0 <= m < 2^32) */
static inline void big_mul_small(BigUint *a, uint32_t m) {
    if (m == 0) { a->len = 0; return; }
    uint64_t carry = 0;
    for (size_t i = 0; i < a->len; ++i) {
        uint64_t prod = (uint64_t)a->d[i] * m + carry;
        a->d[i] = (uint32_t)(prod % BASE);
        carry = prod / BASE;
    }
    if (carry) {
        a->d[a->len++] = (uint32_t)carry;
    }
}

/* a -= b (assume a >= b) */
static inline void big_sub(BigUint *a, const BigUint *b) {
    int64_t carry = 0;
    size_t i = 0;
    for (; i < b->len || carry; ++i) {
        int64_t ai = (i < a->len) ? a->d[i] : 0;
        int64_t bi = (i < b->len) ? b->d[i] : 0;
        int64_t diff = ai - bi - carry;
        if (diff < 0) {
            diff += BASE;
            carry = 1;
        } else {
            carry = 0;
        }
        if (i < a->len) a->d[i] = (uint32_t)diff;
        else a->d[i] = (uint32_t)diff;  /* should not happen for valid subtraction */
    }
    big_trim(a);
}

/* ------------------------------------------------------------------- */
/* Sequence generation                                               */
/* ------------------------------------------------------------------- */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* undefined for offset, but safe */

    BigUint r, p, cand;
    big_zero(&r);
    big_zero(&p);
    /* keep p modulo 100 only */
    uint32_t p_mod100 = 0;

    for (idx_t i = 0; i <= n; ++i) {
        /* r = r*100 + pair_value */
        big_mul_small(&r, 100);
        if (i == 0) big_add_small(&r, 51);  /* first pair is 51 */
        /* find largest x in [0..9] such that (20*p + x)*x <= r */
        int x;
        for (x = 9; x >= 0; --x) {
            big_copy(&cand, &p);       /* cand = p */
            big_mul_small(&cand, 20);   /* cand = 20*p */
            big_add_small(&cand, (uint32_t)x); /* cand += x */
            big_mul_small(&cand, (uint32_t)x); /* cand *= x */
            if (big_cmp(&cand, &r) <= 0) break;
        }
        /* r -= cand */
        big_sub(&r, &cand);
        /* p = p*10 + x */
        big_mul_small(&p, 10);
        big_add_small(&p, (uint32_t)x);
        /* update p_mod100 */
        p_mod100 = (uint32_t)((((uint64_t)p_mod100 * 10U) + (uint32_t)x) % 100U);
    }

    /* compute digit */
    uint32_t digit;
    if (n == 1) {
        uint32_t tmp = (p_mod100 + 100U - 40U) % 100U; /* (p - 40) mod 100 */
        digit = (tmp / 10U) % 10U;
    } else {
        /* for n >= 2, subtraction of 4*10^n affects only higher digits */
        digit = (p_mod100 / 10U) % 10U;
    }
    return (seq_t)digit;
}
