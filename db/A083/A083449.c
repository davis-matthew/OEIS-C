/*
 * A083449  a(n) = A019566(n)/9, where A019566(n) = concat(n,...,1) -
 *          concat(1,...,n).
 * https://oeis.org/A083449
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, sign
 * Formula:   a(n) = (concat(n,…,1) - concat(1,…,n)) / 9
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 10, i64 n <= 15, i128 n <= 19,
 *            big any n
 * Helpers:   digits_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (with concatenation interpreted in decimal). For n = 1 the
   value is 0.)  The implementation uses a minimal base‑1e9
   big‑integer type that supports multiplication by a small int,
   addition of a small int, subtraction and division by a
   small int, all performed with signed intermediate results
   where appropriate.                                           */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/digits_of.h"

/* ---------- Base‑1e9 big integer support ---------- */
#define BIG_DIGIT_BASE 1000000000U

typedef struct {
    uint32_t d[7000];   /* enough for < 50 000 decimal digits */
    size_t   len;
} BigInt;

/* multiply a by a small integer m (0 <= m < BIG_DIGIT_BASE) */
static void big_mul_small(BigInt *a, uint32_t m)
{
    if (a->len == 0 || m == 1) return;          /* nothing to do */
    if (m == 0) { a->len = 0; return; }

    uint64_t carry = 0;
    for (size_t i = 0; i < a->len; ++i) {
        uint64_t prod = (uint64_t)a->d[i] * m + carry;
        a->d[i] = (uint32_t)(prod % BIG_DIGIT_BASE);
        carry   = prod / BIG_DIGIT_BASE;
    }
    while (carry) {
        a->d[a->len++] = (uint32_t)(carry % BIG_DIGIT_BASE);
        carry /= BIG_DIGIT_BASE;
    }
}

/* add a small integer v (0 <= v < BIG_DIGIT_BASE) to a */
static void big_add_small(BigInt *a, uint32_t v)
{
    uint64_t sum = (a->len == 0 ? 0 : (uint64_t)a->d[0]) + v;
    if (a->len == 0) {
        a->len = 1;
        a->d[0] = (uint32_t)(sum % BIG_DIGIT_BASE);
        sum /= BIG_DIGIT_BASE;
    } else {
        a->d[0] = (uint32_t)(sum % BIG_DIGIT_BASE);
    }
    uint64_t carry = sum / BIG_DIGIT_BASE;
    size_t i = 1;
    while (carry && i < a->len) {
        uint64_t s = (uint64_t)a->d[i] + carry;
        a->d[i] = (uint32_t)(s % BIG_DIGIT_BASE);
        carry   = s / BIG_DIGIT_BASE;
        ++i;
    }
    while (carry) {
        a->d[a->len++] = (uint32_t)(carry % BIG_DIGIT_BASE);
        carry /= BIG_DIGIT_BASE;
    }
}

/* compare a and b: 1 if a>b, -1 if a<b, 0 if equal */
static int big_cmp(const BigInt *a, const BigInt *b)
{
    if (a->len != b->len) return (a->len > b->len) ? 1 : -1;
    for (size_t i = a->len; i > 0; --i) {
        if (a->d[i-1] != b->d[i-1])
            return (a->d[i-1] > b->d[i-1]) ? 1 : -1;
    }
    return 0;
}

/* subtract b from a (a>=b); result stored in res (no sign) */
static void big_sub(const BigInt *a, const BigInt *b, BigInt *res)
{
    int64_t borrow = 0;
    for (size_t i = 0; i < a->len; ++i) {
        int64_t diff = (int64_t)a->d[i]
                       - (i < b->len ? b->d[i] : 0)
                       - borrow;
        if (diff < 0) {
            diff += BIG_DIGIT_BASE;
            borrow = 1;
        } else
            borrow = 0;
        res->d[i] = (uint32_t)diff;
    }
    res->len = a->len;
    while (res->len && res->d[res->len-1] == 0) --res->len;
}

/* divide a by a small divisor d (assume exact division); result in res */
static void big_div_small(const BigInt *a, uint32_t d, BigInt *res)
{
    res->len = a->len;
    uint64_t rem = 0;
    size_t i = a->len;
    while (i--) {
        uint64_t cur = (uint64_t)a->d[i] + rem * BIG_DIGIT_BASE;
        uint32_t q   = (uint32_t)(cur / d);
        rem          = cur % d;
        res->d[i]    = q;
    }
    while (res->len && res->d[res->len-1] == 0) --res->len;
}

/* convert big integer to seq_t (assumes result fits in seq_t) */
static seq_t big_to_seq(const BigInt *a)
{
    seq_t val = 0;
    for (size_t i = a->len; i > 0; --i) {
        val = val * (seq_t)BIG_DIGIT_BASE + (seq_t)a->d[i-1];
    }
    return val;
}

/* count decimal digits of an idx_t value */

/* --------- Main sequence function --------- */
seq_t a(idx_t n)
{
    /* powers of 10 up to 5 digits (since n ≤ 10000) */
    static const uint32_t pow10[6] = {1,10,100,1000,10000,100000};

    BigInt b1, b2;
    b1.len = b2.len = 0;

    /* Build concat(n,…,1) */
    for (idx_t i = n; i > 0; --i) {
        int d = digits_of(i);          /* number of decimal digits of i */
        big_mul_small(&b1, pow10[d]);  /* multiply by 10^d */
        big_add_small(&b1, (uint32_t)i);/* add i */
    }

    /* Build concat(1,…,n) */
    for (idx_t i = 1; i <= n; ++i) {
        int d = digits_of(i);
        big_mul_small(&b2, pow10[d]);
        big_add_small(&b2, (uint32_t)i);
    }

    /* Compute |b1-b2| and determine sign */
    bool negative = false;
    BigInt diff;
    if (big_cmp(&b1, &b2) >= 0) {
        big_sub(&b1, &b2, &diff);        /* positive difference */
    } else {
        negative = true;
        big_sub(&b2, &b1, &diff);        /* diff = b2-b1 */
    }

    /* Divide the absolute difference by 9 */
    BigInt q;
    big_div_small(&diff, 9, &q);

    /* Convert to seq_t and apply sign */
    seq_t result = big_to_seq(&q);
    if (negative) result = -result;

    return result;
}
