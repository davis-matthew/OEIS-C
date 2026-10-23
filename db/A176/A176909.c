/*
 * A176909  Decimal expansion of sqrt(230).
 * https://oeis.org/A176909
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

/* Decimal digits of sqrt(230) including the integer part (offset 2). */
/* a(2) = first digit of the integer part (1), a(3) = second integer digit (5),
   a(4) = first decimal digit (1), a(5) = second decimal digit (6), etc. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Big‑integer with base BASE = 10⁹. */
#define BASE 1000000000U
#define MAX_BD 1500   /* enough for ~10000 decimal digits */

typedef struct {
    uint32_t d[MAX_BD];
    size_t sz;       /* number of used limbs; d[0] is least significant */
} bd_t;

/* Basic operations on bd_t */
static void bd_from_uint(bd_t *x, uint32_t v) {
    if (v == 0) {
        x->sz = 0;
        return;
    }
    x->sz = 1;
    x->d[0] = v;
}

static void bd_copy(bd_t *dest, const bd_t *src) {
    dest->sz = src->sz;
    memcpy(dest->d, src->d, src->sz * sizeof(uint32_t));
}

static int bd_cmp(const bd_t *a, const bd_t *b) {
    if (a->sz != b->sz)
        return (a->sz < b->sz) ? -1 : 1;
    for (size_t i = a->sz; i-- > 0;) {
        if (a->d[i] != b->d[i])
            return (a->d[i] < b->d[i]) ? -1 : 1;
    }
    return 0;
}

static void bd_add_small(bd_t *x, uint32_t v) {
    uint64_t carry = v;
    size_t i = 0;
    while (carry && i < x->sz) {
        uint64_t sum = (uint64_t)x->d[i] + carry;
        x->d[i] = (uint32_t)(sum % BASE);
        carry    = sum / BASE;
        ++i;
    }
    if (carry) {
        x->d[x->sz++] = (uint32_t)carry;
    }
}

static void bd_mul_small(bd_t *x, uint32_t m) {
    if (m == 0) {
        x->sz = 0;
        return;
    }
    uint64_t carry = 0;
    for (size_t i = 0; i < x->sz; ++i) {
        uint64_t prod = (uint64_t)x->d[i] * m + carry;
        x->d[i] = (uint32_t)(prod % BASE);
        carry   = prod / BASE;
    }
    while (carry) {
        x->d[x->sz++] = (uint32_t)(carry % BASE);
        carry /= BASE;
    }
}

static void bd_sub(bd_t *x, const bd_t *y) {          /* assume x >= y */
    uint64_t borrow = 0;
    for (size_t i = 0; i < x->sz; ++i) {
        int64_t diff = (int64_t)x->d[i] - borrow -
                       (i < y->sz ? y->d[i] : 0);
        if (diff < 0) {
            diff += BASE;
            borrow = 1;
        } else
            borrow = 0;
        x->d[i] = (uint32_t)diff;
    }
    while (x->sz > 0 && x->d[x->sz - 1] == 0)
        --x->sz;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n < 2)
        return 0;  /* undefined for n < 2 */

    /* integer part digits of sqrt(230) */
    static const seq_t int_digits[2] = { (seq_t)1, (seq_t)5 };

    /* overall 0‑based index relative to first integer digit */
    idx_t idx = n - 2;
    if (idx < 2)
        return int_digits[(size_t)idx];

    /* decimal position (0‑based) */
    size_t dpos = (size_t)(idx - 2);

    /* static cache of generated decimal digits */
    static seq_t *digits = NULL;
    static size_t digits_len = 0, digits_cap = 0;

    /* static big integers used in the expansion algorithm */
    static bd_t p;      /* current prefix (integer part + digits so far) */
    static bd_t rem;    /* current remainder */
    static int initialized = 0;
    if (!initialized) {
        digits_cap = 16;
        digits = (seq_t *)malloc(digits_cap * sizeof(seq_t));
        digits_len = 0;

        bd_from_uint(&p, 15);   /* integer part of sqrt(230) */
        bd_from_uint(&rem, 5);  /* 230 - 15² = 5 */

        initialized = 1;
    }

    /* generate more digits as needed */
    while (digits_len <= dpos) {
        /* r2 = rem * 100 */
        bd_t r2;
        bd_copy(&r2, &rem);
        bd_mul_small(&r2, 100);

        int chosen = 0;
        bd_t temp, prod, rtmp;

        for (int d = 9; d >= 0; --d) {
            /* temp = (20 * p + d) */
            bd_copy(&temp, &p);
            bd_mul_small(&temp, 20);
            bd_add_small(&temp, (uint32_t)d);

            /* prod = temp * d */
            bd_copy(&prod, &temp);
            bd_mul_small(&prod, (uint32_t)d);

            if (bd_cmp(&prod, &r2) <= 0) {
                chosen = d;

                /* rem = r2 - prod */
                bd_copy(&rtmp, &r2);
                bd_sub(&rtmp, &prod);
                bd_copy(&rem, &rtmp);

                /* p = p * 10 + d */
                bd_mul_small(&p, 10);
                bd_add_small(&p, (uint32_t)d);

                /* store the digit */
                if (digits_len == digits_cap) {
                    digits_cap *= 2;
                    digits = (seq_t *)realloc(digits, digits_cap * sizeof(seq_t));
                }
                digits[digits_len++] = (seq_t)chosen;
                break;
            }
        }
        /* It is guaranteed that a digit is found. */
    }

    return digits[dpos];
}
