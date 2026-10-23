/*
 * A010539  Decimal expansion of square root of 88.
 * https://oeis.org/A010539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 9883, u16 n <= 9883, u32 n <= 9883, u64 n <= 9883,
 *            u128 n <= 9883, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The nth term is the nth decimal digit of sqrt(88) computed by the
   classical digit‑by‑digit extraction algorithm. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- minimal big integer implementation ---------- */
#define BASE ((uint32_t)1000000000u)   /* radix 10^9 for big integer digits */

typedef struct {
    uint32_t *d;    /* little‑endian digits, least significant first */
    size_t   len;   /* number of used digits */
    size_t   cap;   /* allocated capacity */
} BigInt;

static void big_ensure(BigInt *b, size_t need)
{
    if (need <= b->cap) return;
    size_t newcap = b->cap ? b->cap : 4;
    while (newcap < need) newcap <<= 1;
    uint32_t *nd = (uint32_t *)realloc(b->d, newcap * sizeof(uint32_t));
    if (!nd) exit(EXIT_FAILURE);
    b->d = nd;
    b->cap = newcap;
}

static void big_init(BigInt *b)
{
    b->len = 0;
    b->cap = 4;
    b->d = (uint32_t *)calloc(b->cap, sizeof(uint32_t));
    if (!b->d) exit(EXIT_FAILURE);
}

static void big_from_uint32(BigInt *b, uint32_t v)
{
    big_ensure(b, 1);
    if (v == 0) {
        b->len = 0;
    } else {
        b->len = 1;
        b->d[0] = v;
    }
}

static void big_copy(BigInt *dest, const BigInt *src)
{
    big_ensure(dest, src->len);
    dest->len = src->len;
    if (src->len)
        memcpy(dest->d, src->d, src->len * sizeof(uint32_t));
}

static void big_mul_small(BigInt *b, uint32_t m)
{
    if (b->len == 0 || m == 1) return;
    if (m == 0) { b->len = 0; return; }
    uint64_t carry = 0;
    for (size_t i = 0; i < b->len; ++i) {
        uint64_t prod = (uint64_t)b->d[i] * m + carry;
        b->d[i] = (uint32_t)(prod % BASE);
        carry = prod / BASE;
    }
    while (carry) {
        big_ensure(b, b->len + 1);
        b->d[b->len++] = (uint32_t)(carry % BASE);
        carry /= BASE;
    }
}

static void big_add_small(BigInt *b, uint32_t v)
{
    uint64_t sum = (b->len ? b->d[0] : 0) + v;
    if (b->len == 0 && sum == 0) return;
    big_ensure(b, b->len ? b->len : 1);
    size_t i = 0;
    if (sum >= BASE) {
        sum -= BASE;
        b->d[0] = (uint32_t)sum;
        sum = 1; /* carry to next digit */
        i = 1;
        while (sum && i < b->len) {
            uint64_t s = (uint64_t)b->d[i] + sum;
            if (s >= BASE) { s -= BASE; sum = 1; } else sum = 0;
            b->d[i++] = (uint32_t)s;
        }
        if (sum) {
            big_ensure(b, b->len + 1);
            b->d[b->len++] = (uint32_t)sum;
        }
    } else {
        b->d[0] = (uint32_t)sum;
    }
}

static int big_compare(const BigInt *a, const BigInt *b)
{
    if (a->len != b->len) return a->len < b->len ? -1 : 1;
    for (size_t i = a->len; i-- > 0; ) {
        if (a->d[i] != b->d[i])
            return a->d[i] < b->d[i] ? -1 : 1;
    }
    return 0;
}

static void big_subtract(BigInt *a, const BigInt *b)
{
    /* assumes a >= b */
    int64_t borrow = 0;
    for (size_t i = 0; i < a->len; ++i) {
        int64_t sub = (int64_t)a->d[i] - (i < b->len ? b->d[i] : 0) - borrow;
        if (sub < 0) {
            sub += BASE;
            borrow = 1;
        } else {
            borrow = 0;
        }
        a->d[i] = (uint32_t)sub;
    }
    while (a->len && a->d[a->len - 1] == 0) --a->len;
}

/* ---------- memoisation state (static globals) ---------- */
static bool init_done = false;
static idx_t computed_max = 0;
static uint8_t *digit_store = NULL;
static size_t digit_cap = 0;

static BigInt root, rem, p, temp;

/* Ensure digit_store is large enough for index n */
static void ensure_digit_capacity(idx_t n)
{
    if ((size_t)(n + 1) > digit_cap) {
        size_t newcap = digit_cap ? digit_cap : 16;
        while (newcap <= (size_t)(n + 1)) newcap <<= 1;
        uint8_t *newptr = (uint8_t *)realloc(digit_store, newcap * sizeof(uint8_t));
        if (!newptr) exit(EXIT_FAILURE);
        digit_store = newptr;
        digit_cap = newcap;
    }
}

/* Initialise root, rem and digit array */
static void init_sequence(void)
{
    big_init(&root);
    big_init(&rem);
    big_init(&p);
    big_init(&temp);

    big_from_uint32(&root, 9u);   /* integer part of sqrt(88) */
    big_from_uint32(&rem, 7u);    /* 88 - 9^2 = 7 */

    digit_cap = 16;
    digit_store = (uint8_t *)malloc(digit_cap * sizeof(uint8_t));
    if (!digit_store) exit(EXIT_FAILURE);
    digit_store[1] = 9u;          /* a(1) = 9 */
    computed_max = 1;
    init_done = true;
}

seq_t a(idx_t n)
{
    if (!init_done) init_sequence();

    if (n == 0) return (seq_t)0;   /* offset is 1; return 0 for safety */

    if (n > computed_max) {
        for (idx_t k = computed_max + 1; k <= n; ++k) {
            /* Step: remainder *= 100 */
            big_mul_small(&rem, 100u);

            /* p = root * 20 */
            big_copy(&p, &root);
            big_mul_small(&p, 20u);

            uint32_t digit = 0;
            for (int d = 9; d >= 0; --d) {
                big_copy(&temp, &p);
                big_add_small(&temp, (uint32_t)d);
                big_mul_small(&temp, (uint32_t)d);
                if (big_compare(&temp, &rem) <= 0) {
                    digit = (uint32_t)d;
                    break;
                }
            }

            /* Update root and remainder */
            big_mul_small(&root, 10u);
            big_add_small(&root, digit);
            big_subtract(&rem, &temp);

            /* Store digit */
            ensure_digit_capacity(k);
            digit_store[k] = (uint8_t)digit;
            computed_max = k;
        }
    }

    return (seq_t)digit_store[n];
}
