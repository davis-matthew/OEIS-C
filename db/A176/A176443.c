/*
 * A176443  Decimal expansion of sqrt(469).
 * https://oeis.org/A176443
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

/* a(n) = the nth decimal digit of sqrt(469)
   (digits of the integer part 21 are a(2)=2, a(3)=1) */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

/* maximum number of decimal digits we expect to compute.
   2048 limbs (1e9 base) give ~18432 decimal digits, plenty for 10 000. */
#define BIGINT_CAP 2048
#define BASE 1000000000U

typedef struct {
    uint32_t d[BIGINT_CAP];
    size_t len;
} BigInt;

static void bi_set_zero(BigInt *x)           { x->len = 0; }
static void bi_set_uint32(BigInt *x, uint32_t v) {
    if (v == 0) { x->len = 0; }
    else { x->d[0] = v; x->len = 1; }
}
static void bi_trim(BigInt *x) {
    while (x->len > 0 && x->d[x->len - 1] == 0) x->len--;
}
static void bi_copy(BigInt *dest, const BigInt *src) {
    dest->len = src->len;
    memcpy(dest->d, src->d, src->len * sizeof(uint32_t));
}
static void bi_mul_small(BigInt *x, uint32_t m) {
    if (m == 0) { x->len = 0; return; }
    if (m == 1 || x->len == 0) return;
    uint64_t carry = 0;
    for (size_t i = 0; i < x->len; ++i) {
        uint64_t prod = (uint64_t)x->d[i] * m + carry;
        x->d[i] = (uint32_t)(prod % BASE);
        carry   = prod / BASE;
    }
    while (carry) {
        x->d[x->len++] = (uint32_t)(carry % BASE);
        carry /= BASE;
    }
}
static void bi_add_small(BigInt *x, uint32_t m) {
    size_t i = 0;
    uint64_t sum = m;
    if (x->len == 0) {
        if (m) { x->d[0] = (uint32_t)m; x->len = 1; }
        return;
    }
    sum += x->d[0];
    x->d[0] = (uint32_t)(sum % BASE);
    uint64_t carry = sum / BASE;
    i = 1;
    while (carry && i < x->len) {
        uint64_t tmp = (uint64_t)x->d[i] + carry;
        x->d[i] = (uint32_t)(tmp % BASE);
        carry = tmp / BASE;
        ++i;
    }
    while (carry) {
        x->d[x->len++] = (uint32_t)(carry % BASE);
        carry /= BASE;
    }
}
static int bi_compare(const BigInt *a, const BigInt *b) {
    if (a->len != b->len) return (a->len > b->len) ? 1 : -1;
    for (size_t i = a->len; i-- > 0;) {
        if (a->d[i] != b->d[i]) return (a->d[i] > b->d[i]) ? 1 : -1;
    }
    return 0;
}
static void bi_subtract(BigInt *a, const BigInt *b) {
    /* assume a >= b */
    uint64_t borrow = 0;
    for (size_t i = 0; i < a->len; ++i) {
        uint64_t sub = borrow + (i < b->len ? b->d[i] : 0);
        if (a->d[i] >= sub) {
            a->d[i] = (uint32_t)(a->d[i] - sub);
            borrow = 0;
        } else {
            a->d[i] = (uint32_t)(BASE + a->d[i] - sub);
            borrow = 1;
        }
    }
    bi_trim(a);
}

/* static state for memoisation */
static seq_t *digits = NULL;
static size_t digits_len = 0;
static size_t digits_cap = 0;
static BigInt big_root;
static BigInt big_rem;
static bool init_done = false;

static void init_state(void) {
    if (init_done) return;

    /* initial root = 21, remainder = 469 - 21^2 = 28 */
    bi_set_uint32(&big_root, 21);
    bi_set_uint32(&big_rem, 28);

    digits_cap = 64;
    digits = (seq_t *)malloc(digits_cap * sizeof(seq_t));
    digits[0] = 2;  /* a(2) */
    digits[1] = 1;  /* a(3) */
    digits_len = 2;

    init_done = true;
}

seq_t a(idx_t n) {
    if (n < 2) return 0;        /* behavior below offset unspecified */
    init_state();

    size_t idx = (size_t)(n - 2);   /* zero‑based index into digits array */
    if (idx < digits_len) return digits[idx];

    /* generate digits up to idx */
    while (digits_len <= idx) {
        /* new_num = remainder * 100 */
        BigInt new_num;
        bi_copy(&new_num, &big_rem);
        bi_mul_small(&new_num, 100);

        /* p = root * 20 */
        BigInt p;
        bi_copy(&p, &big_root);
        bi_mul_small(&p, 20);

        int chosen_d = 0;
        /* test digits 9..0 */
        for (int d = 9; d >= 0; --d) {
            BigInt cand;
            bi_copy(&cand, &p);
            bi_add_small(&cand, (uint32_t)d);
            bi_mul_small(&cand, (uint32_t)d);
            if (bi_compare(&cand, &new_num) <= 0) {
                chosen_d = d;
                /* subtract candidate from new_num to get new remainder */
                bi_copy(&big_rem, &new_num);
                bi_subtract(&big_rem, &cand);
                break;
            }
        }

        digits[digits_len++] = (seq_t)chosen_d;

        /* update root = root * 10 + d */
        bi_mul_small(&big_root, 10);
        bi_add_small(&big_root, (uint32_t)chosen_d);

        /* grow digits array if necessary */
        if (digits_len == digits_cap) {
            digits_cap <<= 1;
            digits = (seq_t *)realloc(digits, digits_cap * sizeof(seq_t));
        }
    }

    return digits[idx];
}
