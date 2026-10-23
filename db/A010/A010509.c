/*
 * A010509  Decimal expansion of square root of 56.
 * https://oeis.org/A010509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = the n-th decimal digit of sqrt(56) (1‑based)
 * Range:     u8 n <= 9829, u16 n <= 9829, u32 n <= 9829, u64 n <= 9829,
 *            u128 n <= 9829, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Big integer in base 1e9; only needed operations are add/sub/mul by small */
typedef struct {
    uint32_t *limb;   /* least‑significant limb first */
    size_t len;       /* number of limbs, len>=1 */
} Big;
#define BIG_BASE 1000000000U

static void big_normalize(Big *a) {
    while (a->len > 1 && a->limb[a->len - 1] == 0)
        a->len--;
}

static Big big_new_unit(uint32_t v) {
    Big a;
    a.limb = (uint32_t*)malloc(sizeof(uint32_t));
    if (!a.limb) abort();
    a.limb[0] = v;
    a.len = 1;
    big_normalize(&a);
    return a;
}

static Big big_copy(const Big *a) {
    Big b;
    b.limb = (uint32_t*)malloc(a->len * sizeof(uint32_t));
    if (!b.limb) abort();
    for (size_t i = 0; i < a->len; i++) b.limb[i] = a->limb[i];
    b.len = a->len;
    return b;
}

static void big_destroy(Big *a) {
    free(a->limb);
    a->limb = NULL;
    a->len = 0;
}

static void big_add_small_inplace(Big *a, uint32_t add) {
    uint64_t sum = (uint64_t)a->limb[0] + add;
    a->limb[0] = (uint32_t)(sum % BIG_BASE);
    uint64_t carry = sum / BIG_BASE;
    size_t i = 1;
    while (carry && i < a->len) {
        sum = (uint64_t)a->limb[i] + carry;
        a->limb[i] = (uint32_t)(sum % BIG_BASE);
        carry = sum / BIG_BASE;
        i++;
    }
    if (carry) {
        a->limb = (uint32_t*)realloc(a->limb, (a->len + 1) * sizeof(uint32_t));
        a->limb[a->len] = (uint32_t)carry;
        a->len++;
    }
}

static void big_mul_small_inplace(Big *a, uint32_t m) {
    if (m == 0) {
        a->len = 1;
        a->limb[0] = 0;
        return;
    }
    uint64_t carry = 0;
    for (size_t i = 0; i < a->len; i++) {
        uint64_t prod = (uint64_t)a->limb[i] * m + carry;
        a->limb[i] = (uint32_t)(prod % BIG_BASE);
        carry = prod / BIG_BASE;
    }
    while (carry) {
        a->limb = (uint32_t*)realloc(a->limb, (a->len + 1) * sizeof(uint32_t));
        a->limb[a->len] = (uint32_t)(carry % BIG_BASE);
        carry /= BIG_BASE;
        a->len++;
    }
}

static Big big_mul_small(const Big *a, uint32_t m) {
    Big r = big_copy(a);
    big_mul_small_inplace(&r, m);
    return r;
}

static Big big_add_small(const Big *a, uint32_t add) {
    Big r = big_copy(a);
    big_add_small_inplace(&r, add);
    return r;
}

static int big_compare(const Big *a, const Big *b) {
    if (a->len != b->len)
        return (a->len > b->len) ? 1 : -1;
    for (size_t i = a->len; i-- > 0; ) {
        if (a->limb[i] != b->limb[i])
            return (a->limb[i] > b->limb[i]) ? 1 : -1;
    }
    return 0;
}

static void big_sub_inplace(Big *a, const Big *b) {
    /* assume a >= b */
    int64_t borrow = 0;
    for (size_t i = 0; i < a->len; i++) {
        int64_t sub = (int64_t)a->limb[i] - borrow;
        if (i < b->len) sub -= b->limb[i];
        if (sub < 0) {
            sub += BIG_BASE;
            borrow = 1;
        } else
            borrow = 0;
        a->limb[i] = (uint32_t)sub;
    }
    big_normalize(a);
}

/* --------------------------------------------------------------------- */

static seq_t *digits = NULL;        /* cached digits, 1‑based stored at index 0 */
static size_t digits_cap = 0;
static size_t digits_len = 0;

/* Big integers storing current root and remainder */
static Big R;     /* current integer root (digits processed so far) */
static Big D;     /* current remainder after producing digits */

/* Initialization flag */
static bool initialized = false;

/* Ensure the cache holds at least n digits. */
static void ensure_len(idx_t n) {
    if (!initialized) {
        /* first digit 7, remainder 56-49=7 */
        digits_cap = 16;
        digits = (seq_t*)malloc(digits_cap * sizeof(seq_t));
        if (!digits) abort();
        digits[0] = 7;
        digits_len = 1;

        R = big_new_unit(7);
        D = big_new_unit(7); /* 56 - 49 */
        initialized = true;
    }
    while (digits_len < (size_t)n) {
        /* Multiply remainder by 100 (bring down two zeros) */
        big_mul_small_inplace(&D, 100);

        /* base = 20 * R */
        Big base = big_mul_small(&R, 20);

        uint32_t chosen = 0;
        for (uint32_t x = 9; ; --x) {
            /* temp = base + x */
            Big temp = big_add_small(&base, x);
            /* candidate = temp * x */
            Big cand = big_mul_small(&temp, x);

            if (big_compare(&cand, &D) <= 0) {
                /* Found digit */
                chosen = x;
                big_sub_inplace(&D, &cand);
                /* R = R * 10 + x */
                big_mul_small_inplace(&R, 10);
                big_add_small_inplace(&R, x);

                if (digits_len == digits_cap) {
                    digits_cap *= 2;
                    digits = (seq_t*)realloc(digits, digits_cap * sizeof(seq_t));
                    if (!digits) abort();
                }
                digits[digits_len++] = (seq_t)x;
                big_destroy(&cand);
                big_destroy(&temp);
                break;
            }
            big_destroy(&cand);
            big_destroy(&temp);
            if (x == 0) break;  /* safety, should never happen */
        }
        big_destroy(&base);
    }
}

/* The required function */
seq_t a(idx_t n) {
    if (n == 0) return 0; /* offset starts at 1, behaviour undefined below */
    ensure_len(n);
    return digits[n - 1];
}
