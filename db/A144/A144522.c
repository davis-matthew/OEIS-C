/*
 * A144522  Sums of pairs of successive digits after the decimal point in the
 *          decimal expansion of 2^(1/2) = 1.41421356...
 * https://oeis.org/A144522
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = digit_(2n-1) + digit_(2n) after the decimal point of
 *            sqrt(2)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation extracts the decimal expansion of sqrt(2) by the
 * classic digit-by-digit method (same as manual extraction) and
 * sums successive non‑overlapping pairs of digits.
 * The arithmetic is performed using a small base‑10^9 big integer
 * implementation that is sufficient for up to 2*n decimal digits
 * (e.g. n = 10000 -> ~20000 digits → < 3000 words). */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define BIGINT_BASE 1000000000UL

struct BigInt {
    uint32_t *d;   /* digits, little-endian, each < BASE */
    size_t len;    /* number of used digits (len==0 means 0) */
    size_t cap;    /* capacity of d array */
};

/* initialise a big integer with given capacity (in words) */
static void bigint_init(struct BigInt *b, size_t cap)
{
    b->d   = (uint32_t *)calloc(cap, sizeof(uint32_t));
    b->len = 0;
    b->cap = cap;
}

/* free the big integer memory */
static void bigint_free(struct BigInt *b)
{
    free(b->d);
    b->len = b->cap = 0;
}

/* set big integer to an unsigned 32‑bit value */
static void bigint_set_uint(struct BigInt *b, uint32_t v)
{
    b->len = (v == 0) ? 0 : 1;
    if (b->len)
        b->d[0] = v;
}

/* copy src into dst (dst must have enough capacity) */
static void bigint_copy(struct BigInt *dst, const struct BigInt *src)
{
    dst->len = src->len;
    memcpy(dst->d, src->d, src->len * sizeof(uint32_t));
}

/* multiply a big integer in place by a small integer (<= UINT32_MAX) */
static void bigint_mul_small(struct BigInt *b, uint32_t m)
{
    if (b->len == 0 || m == 1) return;
    if (m == 0) { b->len = 0; return; }
    uint64_t carry = 0;
    for (size_t i = 0; i < b->len; ++i) {
        uint64_t prod = (uint64_t)b->d[i] * m + carry;
        b->d[i] = (uint32_t)(prod % BIGINT_BASE);
        carry   = prod / BIGINT_BASE;
    }
    while (carry) {
        if (b->len >= b->cap) {
            b->cap *= 2;
            b->d = (uint32_t *)realloc(b->d, b->cap * sizeof(uint32_t));
        }
        b->d[b->len++] = (uint32_t)(carry % BIGINT_BASE);
        carry /= BIGINT_BASE;
    }
}

/* compute dst = src * mul (small integer), dst must have enough capacity */
static void bigint_mul_small_copy(struct BigInt *dst, const struct BigInt *src, uint32_t mul)
{
    bigint_copy(dst, src);
    bigint_mul_small(dst, mul);
}

/* add a small integer (assumed < BASE) to a big integer in place */
static void bigint_add_small(struct BigInt *b, uint32_t add)
{
    if (add == 0) return;
    uint64_t sum = (uint64_t)((b->len > 0) ? b->d[0] : 0) + add;
    b->d[0] = (uint32_t)(sum % BIGINT_BASE);
    uint64_t carry = sum / BIGINT_BASE;
    size_t i = 1;
    while (carry) {
        if (b->len <= i) {
            if (b->len >= b->cap) {
                b->cap *= 2;
                b->d = (uint32_t *)realloc(b->d, b->cap * sizeof(uint32_t));
            }
            b->len = i + 1;
        }
        uint64_t tmp = (uint64_t)b->d[i] + carry;
        b->d[i] = (uint32_t)(tmp % BIGINT_BASE);
        carry   = tmp / BIGINT_BASE;
        ++i;
    }
    if (b->len == 0 && b->d[0] == 0) b->len = 0;
}

/* compare two big integers: return <0 if a<b, 0 if a==b, >0 if a>b */
static int bigint_cmp(const struct BigInt *a, const struct BigInt *b)
{
    if (a->len != b->len)
        return (a->len < b->len) ? -1 : 1;
    for (size_t i = a->len; i-- > 0; ) {
        if (a->d[i] != b->d[i])
            return (a->d[i] < b->d[i]) ? -1 : 1;
    }
    return 0;
}

/* subtract b from a (a must be >= b). Result stored in a. */
static void bigint_sub(struct BigInt *a, const struct BigInt *b)
{
    uint64_t borrow = 0;
    for (size_t i = 0; i < a->len; ++i) {
        uint64_t bi = (i < b->len) ? b->d[i] : 0;
        uint64_t sub = bi + borrow;
        if (a->d[i] >= sub) {
            a->d[i] -= sub;
            borrow = 0;
        } else {
            a->d[i] = (uint32_t)(BIGINT_BASE + a->d[i] - sub);
            borrow = 1;
        }
    }
    /* trim leading zeros */
    while (a->len > 0 && a->d[a->len - 1] == 0)
        --a->len;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* offset starts at 1 */

    /* number of decimal digits to generate */
    idx_t digits_needed = 2 * n;

    /* estimate words needed: each word holds 9 decimal digits */
    size_t cap = (size_t)((digits_needed + 9) / 9 + 5);

    struct BigInt P, R, cand, temp;
    bigint_init(&P, cap);
    bigint_init(&R, cap);
    bigint_init(&cand, cap);
    bigint_init(&temp, cap);

    /* initial state: sqrt(2) = 1.4142... */
    bigint_set_uint(&P, 1);            /* integer part */
    bigint_set_uint(&R, 1);            /* 2 - 1^2 = 1 */

    int prev_digit = -1;  /* digit at previous position */
    seq_t result = 0;

    for (idx_t i = 0; i < digits_needed; ++i) {
        /* enlarge remainder by two decimal places */
        bigint_mul_small(&R, 100);

        /* candidate = 20 * P */
        bigint_mul_small_copy(&cand, &P, 20);

        int digit = 0;
        for (int d = 9; d >= 0; --d) {
            /* temp = cand * d */
            if (d == 0) {
                temp.len = 0;    /* zero */
            } else {
                bigint_mul_small_copy(&temp, &cand, (uint32_t)d);
                /* temp += d*d */
                bigint_add_small(&temp, (uint32_t)(d * d));
            }

            if (bigint_cmp(&temp, &R) <= 0) {
                /* choose this digit */
                digit = d;
                /* R -= temp */
                if (digit != 0)
                    bigint_sub(&R, &temp);
                /* P = P*10 + d */
                bigint_mul_small(&P, 10);
                if (digit != 0)
                    bigint_add_small(&P, (uint32_t)digit);
                break;
            }
        }

        if (i == digits_needed - 1) {
            /* last digit of the pair */
            result = (seq_t)(prev_digit + digit);
            break;
        }
        prev_digit = digit;
    }

    /* clean up */
    bigint_free(&P);
    bigint_free(&R);
    bigint_free(&cand);
    bigint_free(&temp);

    return result;
}
