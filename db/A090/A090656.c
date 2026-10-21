/*
 * A090656  Decimal expansion of 5 + sqrt(35).
 * https://oeis.org/A090656
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 9535, u16 n <= 9535, u32 n <= 9535, u64 n <= 9535,
 *            u128 n <= 9535, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = digits of 5 + sqrt(35) in base 10, written
 * consecutively with no decimal point.  The integer part is 10,
 * so a(2)=1, a(3)=0 and the remaining digits are the decimal
 * expansion of sqrt(35).
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* --------------------------------------------------------------------- */
/* simple bignum: unsigned integer in little‑endian base 2^32            */
/* --------------------------------------------------------------------- */
#define BIGINT_BASE (1ull << 32)

typedef struct {
    uint32_t *d;      /* limbs, little‑endian   */
    size_t   len;     /* number of limbs, len=0 means zero */
} BigInt;

/* zero a BigInt */
static void bigint_zero(BigInt *a) {
    if (a->len) {
        free(a->d);
        a->d   = NULL;
        a->len = 0;
    }
}

/* set BigInt to a small uint64_t value */
static void bigint_set_uint(BigInt *a, uint64_t v) {
    bigint_zero(a);
    if (v == 0) return;
    a->len = 1;
    a->d   = (uint32_t *)malloc(sizeof(uint32_t));
    a->d[0] = (uint32_t)v;
}

/* multiply a by an unsigned 32‑bit integer, in-place */
static void bigint_mul_uint(BigInt *a, uint32_t m) {
    if (m == 0) { bigint_zero(a); return; }
    uint64_t carry = 0;
    for (size_t i = 0; i < a->len; ++i) {
        uint64_t prod = (uint64_t)a->d[i] * m + carry;
        a->d[i] = (uint32_t)(prod & 0xffffffffu);
        carry   = prod >> 32;
    }
    while (carry) {
        a->d = (uint32_t *)realloc(a->d, (a->len + 1) * sizeof(uint32_t));
        a->d[a->len++] = (uint32_t)carry;
        carry >>= 32;
    }
}

/* add an unsigned 32‑bit integer to a, in-place */
static void bigint_add_uint(BigInt *a, uint32_t v) {
    size_t i = 0;
    uint64_t carry = v;
    while (carry && i < a->len) {
        uint64_t tmp = (uint64_t)a->d[i] + carry;
        a->d[i] = (uint32_t)(tmp & 0xffffffffu);
        carry = tmp >> 32;
        ++i;
    }
    while (carry) {
        a->d = (uint32_t *)realloc(a->d, (a->len + 1) * sizeof(uint32_t));
        a->d[a->len++] = (uint32_t)carry;
        carry >>= 32;
    }
}

/* subtract an unsigned 32‑bit integer from a (a >= v) */
static void bigint_sub_uint(BigInt *a, uint32_t v) {
    uint64_t borrow = v;
    size_t i = 0;
    while (borrow && i < a->len) {
        if ((uint64_t)a->d[i] >= borrow) {
            a->d[i] = (uint32_t)((uint64_t)a->d[i] - borrow);
            borrow  = 0;
        } else {
            a->d[i] = (uint32_t)((1ull << 32) + a->d[i] - borrow);
            borrow  = 1;
        }
        ++i;
    }
    /* strip leading zero limbs */
    while (a->len && a->d[a->len - 1] == 0) --a->len;
    if (a->len == 0) free(a->d);
}

/* subtract b from a (a >= b), in-place */
static void bigint_sub_big(BigInt *a, const BigInt *b) {
    size_t i = 0;
    uint64_t borrow = 0;
    while (i < a->len) {
        uint64_t ai = a->d[i];
        uint64_t bi = (i < b->len) ? b->d[i] : 0;
        uint64_t bi_borrow = bi + borrow;
        if (ai >= bi_borrow) {
            a->d[i] = (uint32_t)(ai - bi_borrow);
            borrow  = 0;
        } else {
            a->d[i] = (uint32_t)((1ull << 32) + ai - bi_borrow);
            borrow  = 1;
        }
        ++i;
    }
    /* strip leading zero limbs */
    while (a->len && a->d[a->len - 1] == 0) --a->len;
    if (a->len == 0) free(a->d);
}

/* copy a into dst, reallocating if necessary */
static void bigint_copy(BigInt *dst, const BigInt *src) {
    if (src->len == 0) {
        bigint_zero(dst);
        return;
    }
    dst->len = src->len;
    dst->d   = (uint32_t *)realloc(dst->d, dst->len * sizeof(uint32_t));
    memcpy(dst->d, src->d, dst->len * sizeof(uint32_t));
}

/* compare two BigInt values: <0 if a<b, 0 if equal, >0 if a>b */
static int bigint_cmp(const BigInt *a, const BigInt *b) {
    if (a->len < b->len) return -1;
    if (a->len > b->len) return 1;
    for (size_t i = a->len; i > 0; --i) {
        uint32_t ai = a->d[i - 1];
        uint32_t bi = b->d[i - 1];
        if (ai < bi) return -1;
        if (ai > bi) return 1;
    }
    return 0;
}

/* --------------------------------------------------------------------- */
/* sqrt(35) digit extraction using the digit‑by‑digit algorithm          */
/* --------------------------------------------------------------------- */
static bool sqrt_initialized = false;
static BigInt p, r;            /* current root partial and remainder */
static seq_t *sqrt_digits = NULL; /* array of decimal digits of sqrt(35) */
static size_t sqrt_cap = 0, sqrt_len = 0;

/* initialise the sqrt algorithm and digit array */
static void sqrt_init(void) {
    bigint_zero(&p);
    bigint_zero(&r);
    bigint_set_uint(&r, 35);     /* start remainder with 35 */

    /* integer part of sqrt(35) */
    uint32_t d0 = 5;
    bigint_sub_uint(&r, d0 * d0);   /* r = 35 - 25 = 10 */
    bigint_set_uint(&p, d0);

    sqrt_cap = 64;
    sqrt_digits = (seq_t *)calloc(sqrt_cap, sizeof(seq_t));
    sqrt_len = 0;
    sqrt_initialized = true;
}

/* generate the next decimal digit of sqrt(35) */
static uint32_t sqrt_next_digit(void) {
    bigint_mul_uint(&r, 100);   /* shift remainder by two decimal places */

    for (int d = 9; d >= 0; --d) {
        BigInt temp;
        temp.d   = NULL;
        temp.len = 0;

        bigint_copy(&temp, &p);
        bigint_mul_uint(&temp, 20);            /* temp = 20 * p */
        bigint_add_uint(&temp, (uint32_t)d);   /* temp = 20*p + d */
        bigint_mul_uint(&temp, (uint32_t)d);   /* temp = (20*p + d)*d */

        if (bigint_cmp(&temp, &r) <= 0) {
            /* accept digit d */
            bigint_sub_big(&r, &temp);
            bigint_mul_uint(&p, 10);
            bigint_add_uint(&p, (uint32_t)d);
            if (temp.d) free(temp.d);
            return (uint32_t)d;
        }
        if (temp.d) free(temp.d);
    }
    /* this should never happen */
    return 0;
}

/* --------------------------------------------------------------------- */
/* public sequence function                                           */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n) {
    /* n < 2 is outside the defined offset; return 0 conventionally */
    if (n < 2) return 0;

    /* the integer part of 5 + sqrt(35) is 10 -> digits 1,0 */
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)0;

    /* compute sqrt(35) digits on demand */
    if (!sqrt_initialized) sqrt_init();

    size_t pos = (size_t)(n - 4);   /* zero‑based position in sqrt digits */
    while (sqrt_len <= pos) {
        uint32_t d = sqrt_next_digit();
        if (sqrt_len == sqrt_cap) {
            sqrt_cap *= 2;
            sqrt_digits = (seq_t *)realloc(sqrt_digits, sqrt_cap * sizeof(seq_t));
        }
        sqrt_digits[sqrt_len++] = (seq_t)d;
    }
    return sqrt_digits[pos];
}
