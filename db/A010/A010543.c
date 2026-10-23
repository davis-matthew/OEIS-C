/*
 * A010543  Decimal expansion of square root of 92.
 * https://oeis.org/A010543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = (floor(10^n * sqrt(92))) mod 10
 * Range:     u8 n <= 9931, u16 n <= 9931, u32 n <= 9931, u64 n <= 9931,
 *            u128 n <= 9931, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements digit‑by‑digit extraction of √92 using the long‑division algorithm. */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define BIG_BASE 1000000000U          /* base 10^9 */
#define BIG_MAX_WORDS 3000            /* enough for ~20000 decimal digits */

/* Simple big integer in base BIG_BASE (little endian). */
typedef struct {
    size_t len;                  /* number of used words (len==0 means 0) */
    uint32_t d[BIG_MAX_WORDS];
} Big;

static inline void bigint_init(Big *b, uint32_t v) {
    if (v == 0) { b->len = 0; return; }
    b->len = 1;
    b->d[0] = v;
}

static inline void bigint_trim(Big *b) {
    while (b->len > 0 && b->d[b->len-1] == 0) --b->len;
}

static inline int bigint_cmp(const Big *a, const Big *b) {
    if (a->len > b->len) return 1;
    if (a->len < b->len) return -1;
    for (int i = (int)a->len-1; i >= 0; --i) {
        if (a->d[i] > b->d[i]) return 1;
        if (a->d[i] < b->d[i]) return -1;
    }
    return 0;
}

static inline void bigint_copy(const Big *src, Big *dst) {
    dst->len = src->len;
    memcpy(dst->d, src->d, src->len * sizeof(uint32_t));
}

static inline void bigint_add_small(Big *a, uint32_t v) {
    if (v == 0) return;
    if (a->len == 0) { a->len = 1; a->d[0] = v; return; }
    uint64_t carry = v;
    size_t i = 0;
    while (carry && i < a->len) {
        uint64_t sum = (uint64_t)a->d[i] + carry;
        a->d[i] = (uint32_t)(sum % BIG_BASE);
        carry   = sum / BIG_BASE;
        ++i;
    }
    if (carry) a->d[a->len++] = (uint32_t)carry;
}

static inline void bigint_mul_small(Big *a, uint32_t m) {
    if (m == 1 || a->len == 0) return;
    if (m == 0) { a->len = 0; return; }
    uint64_t carry = 0;
    for (size_t i = 0; i < a->len; ++i) {
        uint64_t prod = (uint64_t)a->d[i] * m + carry;
        a->d[i] = (uint32_t)(prod % BIG_BASE);
        carry   = prod / BIG_BASE;
    }
    while (carry) {
        a->d[a->len++] = (uint32_t)(carry % BIG_BASE);
        carry /= BIG_BASE;
    }
}

static inline void bigint_mul_small_res(const Big *a, uint32_t m, Big *res) {
    res->len = 0;
    if (m == 0 || a->len == 0) return;
    uint64_t carry = 0;
    for (size_t i = 0; i < a->len; ++i) {
        uint64_t prod = (uint64_t)a->d[i] * m + carry;
        res->d[res->len++] = (uint32_t)(prod % BIG_BASE);
        carry   = prod / BIG_BASE;
    }
    while (carry) {
        res->d[res->len++] = (uint32_t)(carry % BIG_BASE);
        carry /= BIG_BASE;
    }
}

static inline void bigint_add_small_res(const Big *a, uint32_t v, Big *res) {
    bigint_copy(a, res);
    bigint_add_small(res, v);
}

static inline void bigint_subtract(Big *a, const Big *b) {
    /* a >= b is assumed */
    uint64_t borrow = 0;
    size_t i = 0;
    while (i < a->len) {
        uint64_t sub = (uint64_t)(i < b->len ? b->d[i] : 0) + borrow;
        if (a->d[i] >= sub) { a->d[i] -= sub; borrow = 0; }
        else               { a->d[i] = a->d[i] + BIG_BASE - sub; borrow = 1; }
        ++i;
    }
    bigint_trim(a);
}

/* ---------  state for digit cache and long‑division --------- */
static uint8_t *digit_cache = NULL;   /* digit_cache[0] = a(1) */
static size_t cache_cap   = 0;
static size_t cache_filled = 0;

static Big r_buf, c_buf, tmp_buf, cand_buf;
static int big_initialized = 0;

static inline void big_init_state(void) {
    bigint_init(&r_buf, 92);   /* first remainder is the full integer 92 */
    bigint_init(&c_buf, 0);    /* root carries the digits already extracted */
    cache_filled = 0;
    big_initialized = 1;
}

/* ---------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1, undefined for n < 1 */

    /* grow digit cache if required */
    if (cache_cap < (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap : 64;
        while (new_cap < (size_t)n) new_cap <<= 1;
        uint8_t *tmp = (uint8_t *)realloc(digit_cache, new_cap * sizeof(uint8_t));
        if (!tmp) return (seq_t)0;               /* allocation failure – not expected */
        digit_cache = tmp;
        cache_cap   = new_cap;
    }

    /* initialise algorithm state on first call */
    if (!big_initialized)
        big_init_state();

    /* compute digits up to index n */
    for (size_t pos = cache_filled; pos < (size_t)n; ++pos) {
        if (pos > 0)                 /* bring down two zeros after the first digit */
            bigint_mul_small(&r_buf, 100);

        int found = 0;
        for (int d = 9; d >= 0; --d) {
            /* tmp = c * (20*d) */
            if (d == 0) {
                tmp_buf.len = 0;
            } else {
                uint32_t mult = 20u * (uint32_t)d;
                bigint_mul_small_res(&c_buf, mult, &tmp_buf);
            }

            /* cand = tmp + d*d */
            uint32_t dd = (uint32_t)(d * d);
            bigint_add_small_res(&tmp_buf, dd, &cand_buf);

            if (bigint_cmp(&cand_buf, &r_buf) <= 0) {
                /* d is the next decimal digit */
                digit_cache[pos] = (uint8_t)d;
                bigint_subtract(&r_buf, &cand_buf);
                bigint_mul_small(&c_buf, 10);
                bigint_add_small(&c_buf, d);
                found = 1;
                break;
            }
        }

        if (!found)                    /* theoretically impossible */
            digit_cache[pos] = 0;
    }

    cache_filled = n;
    return (seq_t)digit_cache[n - 1];
}
