/*
 * A187109  Decimal expansion of sqrt(2/11).
 * https://oeis.org/A187109
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(sqrt(2/11) * 10^(n+1)) mod 10
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   alloc_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/alloc_seq_array.h"

/* --------------------------------------------------------------------- */
/*  Simple arbitrary‑precision integers in base 1e9                       */
/* --------------------------------------------------------------------- */
#define MAX_WORDS 2500           /* enough for 20000 decimal digits */
#define BASE 1000000000U

typedef struct {
    size_t len;                 /* number of used words (little‑endian) */
    uint32_t d[MAX_WORDS];
} BigInt;

static inline void bigint_init(BigInt *x) { x->len = 0; }

static inline void bigint_from_uint(BigInt *x, uint32_t v) {
    if (v == 0) {
        x->len = 0;
    } else {
        x->len = 1;
        x->d[0] = v;
    }
}

static inline void bigint_norm(BigInt *x) {
    while (x->len && x->d[x->len - 1] == 0) x->len--;
}

static inline void bigint_copy(const BigInt *src, BigInt *dst) {
    dst->len = src->len;
    if (src->len) memcpy(dst->d, src->d, src->len * sizeof(uint32_t));
}

static inline void bigint_add_small(BigInt *x, uint32_t v) {
    uint64_t carry = v;
    size_t i = 0;
    while (carry && i < x->len) {
        uint64_t sum = (uint64_t)x->d[i] + carry;
        x->d[i] = (uint32_t)(sum % BASE);
        carry   = sum / BASE;
        ++i;
    }
    if (carry) {
        if (x->len < MAX_WORDS) x->d[x->len++] = (uint32_t)carry;
    }
}

static inline void bigint_mul_small(BigInt *x, uint32_t m) {
    if (m == 0 || x->len == 0) { x->len = 0; return; }
    uint64_t carry = 0;
    for (size_t i = 0; i < x->len; ++i) {
        uint64_t prod = (uint64_t)x->d[i] * m + carry;
        x->d[i] = (uint32_t)(prod % BASE);
        carry   = prod / BASE;
    }
    if (carry) {
        if (x->len < MAX_WORDS) x->d[x->len++] = (uint32_t)carry;
    }
}

static inline int bigint_cmp(const BigInt *a, const BigInt *b) {
    if (a->len > b->len) return 1;
    if (a->len < b->len) return -1;
    for (size_t i = a->len; i-- > 0;) {
        if (a->d[i] > b->d[i]) return 1;
        if (a->d[i] < b->d[i]) return -1;
    }
    return 0;
}

static inline void bigint_sub(BigInt *x, const BigInt *y) { /* assumes x >= y */
    uint64_t borrow = 0;
    size_t i = 0;
    for (; i < y->len; ++i) {
        uint64_t sub = (uint64_t)x->d[i] - y->d[i] - borrow;
        if ((int64_t)sub < 0) {
            sub += BASE;
            borrow = 1;
        } else borrow = 0;
        x->d[i] = (uint32_t)sub;
    }
    while (borrow && i < x->len) {
        uint64_t sub = (uint64_t)x->d[i] - borrow;
        if ((int64_t)sub < 0) {
            sub += BASE;
            borrow = 1;
        } else borrow = 0;
        x->d[i] = (uint32_t)sub;
        ++i;
    }
    bigint_norm(x);
}

/* --------------------------------------------------------------------- */
/*  State for the digit generator                                       */
/* --------------------------------------------------------------------- */
static seq_t *digit_cache = NULL;
static size_t digit_cap = 0;
static size_t digit_len = 0;

static BigInt p_root;          /* partial root */
static BigInt r_rem;           /* remainder in algorithm */
static uint64_t rem_frac;      /* remainder of the fraction 2/11 */
static const uint64_t NUM = 2ULL;
static const uint64_t DEN = 11ULL;
static bool initialized = false;

/* Ensure that digit_cache[0..n] are valid */
static void ensure_digits(idx_t n) {
    if (!initialized) {
        bigint_init(&p_root);
        bigint_init(&r_rem);
        rem_frac = NUM % DEN;            /* 2 % 11 = 2 */
        /* bring down first two digits of 2/11: 18 */
        uint64_t tmp = rem_frac * 100ULL;
        uint32_t group = (uint32_t)(tmp / DEN);   /* 18 */
        rem_frac = (tmp % DEN);
        bigint_from_uint(&r_rem, group);
        digit_len = 0;
        digit_cap = 64;
        digit_cache = alloc_seq_array(digit_cap);
        initialized = true;
    }

    /* Grow cache if needed */
    while (digit_len <= (size_t)n) {
        if (digit_len == digit_cap) {
            seq_t *tmp = alloc_seq_array(digit_cap * 2);
            memcpy(tmp, digit_cache, digit_cap * sizeof(seq_t));
            free(digit_cache);
            digit_cache = tmp;
            digit_cap *= 2;
        }

        /* Compute next digit */
        /* 1. Find d: (20*p + d)*d <= r */
        BigInt c, prod, temp;
        bigint_copy(&p_root, &c);
        bigint_mul_small(&c, 20U);          /* c = 20 * p */
        uint32_t chosen_d = 0;
        for (uint32_t d = 9; ; --d) {
            bigint_copy(&c, &temp);
            bigint_add_small(&temp, d);      /* temp = 20p + d */
            bigint_mul_small(&temp, d);      /* temp = (20p + d) * d */
            if (bigint_cmp(&temp, &r_rem) <= 0) {
                chosen_d = d;
                prod = temp;                /* keep product for subtraction */
                break;
            }
            if (d == 0) break;
        }

        /* 2. Update p: p = p*10 + d */
        bigint_mul_small(&p_root, 10U);
        bigint_add_small(&p_root, chosen_d);

        /* 3. Subtract prod from r */
        bigint_sub(&r_rem, &prod);

        /* 4. Bring down next two digits from the fraction */
        uint64_t tmp = rem_frac * 100ULL;
        uint32_t next_group = (uint32_t)(tmp / DEN);
        rem_frac = (tmp % DEN);
        bigint_mul_small(&r_rem, 100U);
        bigint_add_small(&r_rem, next_group);

        /* 5. Store digit */
        digit_cache[digit_len++] = (seq_t)chosen_d;
    }
}

seq_t a(idx_t n) {
    ensure_digits(n);
    return digit_cache[(size_t)n];
}
