/*
 * A010496  Decimal expansion of square root of 42.
 * https://oeis.org/A010496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 9576, u16 n <= 9576, u32 n <= 9576, u64 n <= 9576,
 *            u128 n <= 9576, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: digit‐by‐digit extraction of the decimal expansion of sqrt(42).
   Uses a small big‑integer type (array of 32‑bit limbs) for the intermediate
   remainder and root, so it works for any seq_t (including 64‑bit)
   while keeping the output digit in seq_t. */
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* --- tiny big integer implementation ------------------------------------- */
typedef struct {
    size_t len;        /* number of limbs (non‑zero) */
    uint32_t *x;       /* little‑endian limbs */
} BigInt;

/* normalize to remove leading zero limbs */
static void big_norm(BigInt *a)
{
    while (a->len > 1 && a->x[a->len-1] == 0) a->len--;
}

/* create a big integer from an unsigned 32‑bit value */
static void big_from_uint32(BigInt *a, uint32_t v)
{
    a->len = 1;
    a->x = (uint32_t*)malloc(sizeof(uint32_t));
    a->x[0] = v;
    big_norm(a);
}

/* copy big integer b into a (allocates a's limb array) */
static void big_copy(BigInt *a, const BigInt *b)
{
    a->len = b->len;
    a->x = (uint32_t*)malloc(a->len * sizeof(uint32_t));
    memcpy(a->x, b->x, a->len * sizeof(uint32_t));
    big_norm(a);
}

/* free the limb array of a (leaving a->len==0) */
static void big_free(BigInt *a)
{
    if (a->x) {
        free(a->x);
        a->x = NULL;
    }
    a->len = 0;
}

/* multiply big integer a by a 32‑bit small value */
static void big_mul_small(BigInt *a, uint32_t m)
{
    if (m == 1) return;
    uint64_t carry = 0;
    for (size_t i = 0; i < a->len; ++i) {
        uint64_t prod = (uint64_t)a->x[i] * m + carry;
        a->x[i] = (uint32_t)prod;
        carry = prod >> 32;
    }
    if (carry) {
        a->x = (uint32_t*)realloc(a->x, (a->len + 1) * sizeof(uint32_t));
        a->x[a->len++] = (uint32_t)carry;
    }
    big_norm(a);
}

/* add a 32‑bit small value to big integer a */
static void big_add_small(BigInt *a, uint32_t add)
{
    uint64_t sum = (uint64_t)a->x[0] + add;
    a->x[0] = (uint32_t)sum;
    uint64_t carry = sum >> 32;
    size_t i = 1;
    while (carry && i < a->len) {
        uint64_t s = (uint64_t)a->x[i] + carry;
        a->x[i] = (uint32_t)s;
        carry = s >> 32;
        ++i;
    }
    if (carry) {
        a->x = (uint32_t*)realloc(a->x, (a->len + 1) * sizeof(uint32_t));
        a->x[a->len++] = (uint32_t)carry;
    }
    big_norm(a);
}

/* subtract big integer b from a (assumes a >= b) */
static void big_subtract(BigInt *a, const BigInt *b)
{
    int64_t borrow = 0;
    for (size_t i = 0; i < a->len; ++i) {
        int64_t ai = (int64_t)a->x[i];
        int64_t bi = (i < b->len) ? (int64_t)b->x[i] : 0;
        int64_t diff = ai - bi - borrow;
        if (diff < 0) {
            diff += ((int64_t)1 << 32);
            borrow = 1;
        } else {
            borrow = 0;
        }
        a->x[i] = (uint32_t)diff;
    }
    /* borrow must be 0 here if a >= b */
    big_norm(a);
}

/* compare a and b: return -1,0,1 for a<b,a==b,a>b */
static int big_compare(const BigInt *a, const BigInt *b)
{
    if (a->len != b->len) return (a->len > b->len) ? 1 : -1;
    for (size_t i = a->len; i-- > 0;) {
        if (a->x[i] != b->x[i]) return (a->x[i] > b->x[i]) ? 1 : -1;
    }
    return 0;
}

/* create big integer dest = src * m */
static void big_mul_small_copy(BigInt *dest, const BigInt *src, uint32_t m)
{
    big_copy(dest, src);
    big_mul_small(dest, m);
}

/* create big integer dest = src + add */
static void big_add_small_copy(BigInt *dest, const BigInt *src, uint32_t add)
{
    big_copy(dest, src);
    big_add_small(dest, add);
}

/* ------------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Return the first digit (the integer part) */
    if (n <= 1u) return (seq_t)6;

    /* Static big integers used across calls */
    static BigInt root, rem;
    static int initialized = 0;

    /* Static digit buffer */
    static uint8_t *digits = NULL;     /* digits after the decimal point */
    static size_t digits_len = 0;
    static size_t digits_cap = 0;

    if (!initialized) {
        /* initialize big integers */
        big_from_uint32(&root, 6u);
        big_from_uint32(&rem, 6u);  /* 42 - 6^2 = 6 */
        initialized = 1;
    }

    /* Grow digit buffer if necessary */
    if (!digits) {
        digits_cap = 16;
        digits = (uint8_t*)malloc(digits_cap * sizeof(uint8_t));
    }

    /* Generate digits up to n-1 (since digits[0] is the first decimal digit) */
    for (size_t i = digits_len; i < (size_t)(n - 1); ++i) {
        /* rem = rem * 100 */
        big_mul_small(&rem, 100u);

        /* c = 20 * root */
        BigInt c;
        big_mul_small_copy(&c, &root, 20u);

        /* find largest d in [0,9] such that (c + d)*d <= rem */
        int chosen = 0;
        for (int d = 9; d >= 0; --d) {
            BigInt tmp1, tmp2;
            big_add_small_copy(&tmp1, &c, (uint32_t)d);
            big_mul_small_copy(&tmp2, &tmp1, (uint32_t)d);
            if (big_compare(&tmp2, &rem) <= 0) {
                chosen = d;
                /* free temporaries and break */
                big_free(&tmp2);
                big_free(&tmp1);
                break;
            }
            big_free(&tmp2);
            big_free(&tmp1);
        }

        /* Update rem: rem -= (c + chosen)*chosen */
        BigInt used;
        BigInt tmp1, tmp2;
        big_add_small_copy(&tmp1, &c, (uint32_t)chosen);
        big_mul_small_copy(&tmp2, &tmp1, (uint32_t)chosen);
        big_subtract(&rem, &tmp2);
        big_free(&tmp2);
        big_free(&tmp1);

        /* Update root: root = root*10 + chosen */
        big_mul_small(&root, 10u);
        big_add_small(&root, (uint32_t)chosen);

        /* Store the digit */
        if (digits_len + 1 > digits_cap) {
            digits_cap <<= 1;
            if (digits_cap == 0) digits_cap = 16;
            digits = (uint8_t*)realloc(digits, digits_cap * sizeof(uint8_t));
        }
        digits[digits_len++] = (uint8_t)chosen;

        /* free c */
        big_free(&c);
    }

    /* return the requested digit */
    return (seq_t)digits[n - 2];
}
