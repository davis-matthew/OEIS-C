/*
 * A031356  10-multiplicative persistence: number of iterations of "multiply
 *          10th powers of digits" needed to reach 0 or 1.
 * https://oeis.org/A031356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of iterations of multiplying the 10th powers of
 *            the decimal digits of n until the result becomes 0 or 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#define BASE 1000000000u   /* 1e9, used as the base for the big integer representation */

/* --------------------------------------------------------------------- */
/*  Big‑integer helper (base 10^9)                                         */
typedef struct {
    uint32_t *limb;  /* least‑significant limb first */
    size_t    len;   /* number of used limbs; len==0 represents 0 */
} BigInt;

static void big_init(BigInt *b, size_t len)
{
    b->limb = (uint32_t *)malloc(len * sizeof(uint32_t));
    b->len  = len;
}

static void big_free(BigInt *b)
{
    if (b->limb) {
        free(b->limb);
        b->limb = NULL;
    }
    b->len = 0;
}

static void big_from_uint64(BigInt *b, uint64_t n)
{
    if (n == 0) {
        b->len  = 0;
        b->limb = NULL;
        return;
    }
    uint32_t tmp[3];
    size_t   i = 0;
    while (n) {
        tmp[i++] = (uint32_t)(n % BASE);
        n /= BASE;
    }
    big_init(b, i);
    for (size_t j = 0; j < i; ++j)
        b->limb[j] = tmp[j];
}

static void big_mul_small(const BigInt *a, uint32_t m, BigInt *res)
{
    if (a->len == 0 || m == 0) {
        res->len  = 0;
        res->limb = NULL;
        return;
    }
    uint64_t carry = 0;
    size_t   i;
    for (i = 0; i < a->len; ++i) {
        uint64_t prod = (uint64_t)a->limb[i] * m + carry;
        res->limb[i]  = (uint32_t)(prod % BASE);
        carry         = prod / BASE;
    }
    if (carry) {
        res->limb[i++] = (uint32_t)carry;
    }
    res->len = i;
}

/* pow10[k] = 10^k  (k = 0..9) */
static const uint32_t POW10[10] = {
    1u, 10u, 100u, 1000u, 10000u,
    100000u, 1000000u, 10000000u, 100000000u, 1000000000u
};

/* Convert a big integer to an array of decimal digits (most significant first). */
static void big_to_digits(const BigInt *b, uint8_t *buf, size_t *len_out)
{
    size_t pos = 0;
    for (size_t idx = b->len; idx > 0; --idx) {
        uint32_t block = b->limb[idx - 1];
        if (idx == b->len) {
            /* most significant block – no leading zeros */
            uint32_t tmp = block;
            uint8_t  tmpbuf[10];
            int      tlen = 0;
            do {
                tmpbuf[tlen++] = (uint8_t)(tmp % 10);
                tmp /= 10;
            } while (tmp);
            for (int j = tlen - 1; j >= 0; --j)
                buf[pos++] = tmpbuf[j];
        } else {
            /* other blocks – exactly 9 digits, with leading zeros */
            for (int k = 8; k >= 0; --k) {
                uint32_t d = (block / POW10[k]) % 10;
                buf[pos++] = (uint8_t)d;
            }
        }
    }
    *len_out = pos;
}

/* Check if the big integer is 0 or 1. */
static bool big_is_zero_or_one(const BigInt *b)
{
    if (b->len == 0) return true;                /* 0 */
    if (b->len == 1 && b->limb[0] <= 1u) return true;  /* 1 */
    return false;
}

/* --------------------------------------------------------------------- */
/*  Precomputed 10th powers of the digits 0‑9                            */
static uint32_t DIGIT_POW10[10];
static bool DIGIT_POW10_INIT = false;

static void init_digit_pow10(void)
{
    if (!DIGIT_POW10_INIT) {
        for (int d = 0; d < 10; ++d) {
            uint64_t r = 1;
            for (int i = 0; i < 10; ++i) r *= (uint64_t)d;
            DIGIT_POW10[d] = (uint32_t)r;   /* 9^10 fits in 32 bits */
        }
        DIGIT_POW10_INIT = true;
    }
}

/* --------------------------------------------------------------------- */
/*  Main function                                                          */
seq_t a(idx_t n)
{
    if (n <= 1) return 0;          /* a(1) = 0, a(0) irrelevant */
    init_digit_pow10();

    BigInt cur;
    big_from_uint64(&cur, (uint64_t)n);

    seq_t steps = 0;
    /* temporary buffer for decimal digits (worst case < 20000 digits for n ≤ 10000) */
    uint8_t digits[20000];
    size_t  dlen;

    while (!big_is_zero_or_one(&cur)) {
        /* get decimal digits of current value */
        big_to_digits(&cur, digits, &dlen);

        /* if any digit is 0 -> next product is 0, stop after this iteration */
        bool has_zero = false;
        for (size_t i = 0; i < dlen; ++i) {
            if (digits[i] == 0) {
                has_zero = true;
                break;
            }
        }
        if (has_zero) {
            ++steps;
            break;
        }

        /* compute product of 10th powers of the digits */
        BigInt prod;
        big_init(&prod, 1);
        prod.limb[0] = 1;
        prod.len     = 1;

        for (size_t i = 0; i < dlen; ++i) {
            BigInt tmp;
            big_init(&tmp, prod.len + 1);
            big_mul_small(&prod, DIGIT_POW10[digits[i]], &tmp);
            big_free(&prod);
            prod = tmp;
        }

        ++steps;
        /* move to next current value */
        big_free(&cur);
        cur = prod;
    }

    big_free(&cur);
    return steps;
}
