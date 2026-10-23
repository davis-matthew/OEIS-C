/*
 * A010603  Decimal expansion of cube root of 32.
 * https://oeis.org/A010603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: digit‑by‑digit cube‑root extraction of 32 yields the nth decimal
 * digit of √[3]{32} with offset 1, returning the digit as seq_t. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
/* Big‑integer type – base 1 000 000 000 */
#define BI_BASE          1000000000U
#define BI_MAX_DIGITS    5000

typedef struct {
    size_t len;                   /* number of used limbs (little endian) */
    uint32_t d[BI_MAX_DIGITS];   /* limbs */
} BI;

/* ---------- big‑integer helpers ------------------------------------------- */
static void bi_zero(BI *x)               { x->len = 0; }

static void bi_set_uint64(BI *x, uint64_t v){
    if (v == 0){ x->len = 0; return; }
    x->len = 0;
    while(v){
        x->d[x->len++] = (uint32_t)(v % BI_BASE);
        v /= BI_BASE;
    }
}

static void bi_copy(BI *dst, const BI *src){
    dst->len = src->len;
    memcpy(dst->d, src->d, src->len * sizeof(uint32_t));
}

static int bi_cmp(const BI *a, const BI *b){
    if (a->len != b->len) return (a->len < b->len) ? -1 : 1;
    for (size_t i = a->len; i-- > 0; ){
        if (a->d[i] != b->d[i]) return (a->d[i] < b->d[i]) ? -1 : 1;
    }
    return 0;
}

/* Multiply a by small m, store in r.  r and src may be the same. */
static void bi_mul_small(BI *r, const BI *a, uint32_t m){
    if (m == 0 || a->len == 0){ r->len = 0; return; }
    r->len = 0;
    uint64_t carry = 0;
    for (size_t i = 0; i < a->len; ++i){
        uint64_t cur = (uint64_t)a->d[i] * m + carry;
        r->d[r->len++] = (uint32_t)(cur % BI_BASE);
        carry = cur / BI_BASE;
    }
    if (carry) r->d[r->len++] = (uint32_t)carry;
}

/* Add small v to a.  v < BI_BASE. */
static void bi_add_small(BI *a, uint32_t v){
    uint64_t carry = v;
    size_t i = 0;
    while(carry){
        if (i == a->len) a->d[a->len++] = 0;
        uint64_t sum = (uint64_t)a->d[i] + carry;
        a->d[i] = (uint32_t)(sum % BI_BASE);
        carry = sum / BI_BASE;
        ++i;
    }
}

/* a += b */
static void bi_add_assign(BI *a, const BI *b){
    uint64_t carry = 0;
    size_t maxlen = a->len > b->len ? a->len : b->len;
    for (size_t i = 0; i < maxlen || carry; ++i){
        if (i == a->len) a->d[a->len++] = 0;
        uint64_t sum = (uint64_t)a->d[i] + (i < b->len ? b->d[i] : 0) + carry;
        a->d[i] = (uint32_t)(sum % BI_BASE);
        carry = sum / BI_BASE;
    }
}

/* a -= b  (assumes a >= b) */
static void bi_sub_assign(BI *a, const BI *b){
    int64_t borrow = 0;
    for (size_t i = 0; i < a->len; ++i){
        int64_t diff = (int64_t)a->d[i] - (i < b->len ? b->d[i] : 0) - borrow;
        if (diff < 0){ diff += BI_BASE; borrow = 1; } else borrow = 0;
        a->d[i] = (uint32_t)diff;
    }
    while(a->len && a->d[a->len-1] == 0) --a->len;
}
/* -------------------------------------------------------------------------- */

/* Return the nth decimal digit (offset 1) of the cube root of 32. */
seq_t a(idx_t n)
{
    /* Special case for the integer part */
    if (n == 1) return (seq_t)3;

    /* big integers used in the extraction algorithm */
    BI P, P2, R, oldP, oldP2, term, tmp1, tmp2, newP, newP2;

    bi_zero(&P); bi_zero(&P2); bi_zero(&R);
    bi_zero(&oldP); bi_zero(&oldP2); bi_zero(&term);
    bi_zero(&tmp1); bi_zero(&tmp2); bi_zero(&newP); bi_zero(&newP2);

    /* Initial root 3 and remainder R = 32 – 3^3 = 5 */
    bi_set_uint64(&P, 3);
    bi_set_uint64(&P2, 9);   /* 3^2 */
    bi_set_uint64(&R, 5);

    /* Loop for each subsequent digit */
    for (idx_t step = 2; step <= n; ++step) {
        /* Bring down the next block of 3 zeros: R *= 1000 */
        bi_mul_small(&tmp1, &R, 1000);
        bi_copy(&R, &tmp1);

        /* Find the largest digit d with
           300*P^2*d + 30*P*d^2 + d^3 <= R
        */
        int d;
        for (d = 9; d >= 0; --d) {
            bi_zero(&term);

            /* term1 = P^2 * 300 * d */
            bi_mul_small(&tmp1, &P2, 300);
            bi_mul_small(&tmp2, &tmp1, (uint32_t)d);
            bi_add_assign(&term, &tmp2);

            /* term2 = P * 30 * d^2 */
            uint32_t d2 = (uint32_t)d * (uint32_t)d;
            bi_mul_small(&tmp1, &P, 30);
            bi_mul_small(&tmp2, &tmp1, d2);
            bi_add_assign(&term, &tmp2);

            /* term3 = d^3 */
            uint32_t d3 = d2 * (uint32_t)d;
            bi_add_small(&term, d3);

            if (bi_cmp(&term, &R) <= 0) break;
        }

        /* If this is the requested position, return the digit. */
        if (step == n) return (seq_t)d;

        /* R -= term */
        bi_sub_assign(&R, &term);

        /* Save current root values before updating */
        bi_copy(&oldP, &P);
        bi_copy(&oldP2, &P2);

        /* Update root: P = oldP*10 + d */
        bi_mul_small(&newP, &oldP, 10);
        bi_add_small(&newP, (uint32_t)d);
        bi_copy(&P, &newP);

        /* Update squared root: P2 = 100*oldP2 + 20*oldP*d + d^2 */
        bi_mul_small(&newP2, &oldP2, 100);
        bi_mul_small(&tmp1, &oldP, 20);
        bi_mul_small(&tmp2, &tmp1, (uint32_t)d);
        bi_add_assign(&newP2, &tmp2);
        bi_add_small(&newP2, (uint32_t)d * (uint32_t)d);
        bi_copy(&P2, &newP2);
    }

    /* Unreachable – compiler requires a return. */
    return (seq_t)0;
}
