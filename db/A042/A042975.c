/*
 * A042975  Decimal expansion of sqrt(0.121121112...).
 * https://oeis.org/A042975
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   sqrt(0.121121112...) (decimal expansion); a(n) = n‑th decimal
 *            digit, * with a(0) = first digit after the point. Therefore *
 *            a(0)=3, a(1)=4, a(2)=8, … (see OEIS A042975)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* internal big integer type (base 1e9) */
#define BIGBASE 1000000000u
#define BIGMAX 2500  /* enough for 20000 decimal digits */

typedef struct {
    size_t sz;                    /* number of used limbs (little endian) */
    uint32_t d[BIGMAX];
} BigInt;

/* set big int to zero */
static inline void bi_zero(BigInt *x) { x->sz = 1; x->d[0] = 0; }

/* set big int to integer value v (0 <= v < BIGBASE) */
static inline void bi_set_uint(BigInt *x, uint32_t v) {
    x->sz = 1;
    x->d[0] = v;
}

/* copy */
static inline void bi_copy(BigInt *dst, const BigInt *src) {
    dst->sz = src->sz;
    memcpy(dst->d, src->d, src->sz * sizeof(uint32_t));
}

/* multiply big int by small integer m (m < BIGBASE) */
static inline void bi_mul_small(BigInt *x, uint32_t m) {
    uint64_t carry = 0;
    for (size_t i = 0; i < x->sz; ++i) {
        uint64_t prod = (uint64_t)x->d[i] * m + carry;
        x->d[i] = (uint32_t)(prod % BIGBASE);
        carry = prod / BIGBASE;
    }
    while (carry) {
        x->d[x->sz++] = (uint32_t)(carry % BIGBASE);
        carry /= BIGBASE;
    }
}

/* add small integer v (v < BIGBASE) */
static inline void bi_add_small(BigInt *x, uint32_t v) {
    uint64_t sum = (uint64_t)x->d[0] + v;
    x->d[0] = (uint32_t)(sum % BIGBASE);
    uint64_t carry = sum / BIGBASE;
    size_t i = 1;
    while (carry) {
        if (i >= x->sz) { x->d[x->sz++] = 0; }
        uint64_t s = (uint64_t)x->d[i] + carry;
        x->d[i] = (uint32_t)(s % BIGBASE);
        carry = s / BIGBASE;
        ++i;
    }
}

/* comparison: returns <0 if a<b, 0 if equal, >0 if a>b */
static inline int bi_cmp(const BigInt *a, const BigInt *b) {
    if (a->sz != b->sz) return (a->sz < b->sz) ? -1 : 1;
    for (size_t i = a->sz; i-- > 0;) {
        if (a->d[i] != b->d[i]) return (a->d[i] < b->d[i]) ? -1 : 1;
    }
    return 0;
}

/* subtract b from a (assumes a >= b) */
static inline void bi_sub(BigInt *a, const BigInt *b) {
    int64_t borrow = 0;
    for (size_t i = 0; i < a->sz; ++i) {
        int64_t ai = a->d[i];
        int64_t bi = (i < b->sz) ? b->d[i] : 0;
        int64_t diff = ai - bi - borrow;
        if (diff < 0) {
            diff += BIGBASE;
            borrow = 1;
        } else borrow = 0;
        a->d[i] = (uint32_t)diff;
    }
    while (a->sz > 1 && a->d[a->sz - 1] == 0) --a->sz;
}

/* cached digits */
static size_t cached_max_n = 0;
static uint8_t *cached_digits = NULL;

/* generate all digits up to index n (inclusive) */
static void ensure_cached(idx_t n) {
    if (cached_max_n > n) return;

    size_t need_digits = (size_t)n + 1;               /* how many output digits   */
    size_t need_s_digits = 2 * need_digits;           /* decimal digits of S needed   */

    uint8_t *s_digits = (uint8_t *)malloc(need_s_digits);
    if (!s_digits) return;

    /* initialise S digits to 1 */
    memset(s_digits, 1, need_s_digits);

    /* add 1 at positions m = k*(k+3)/2 (1‑based) */
    for (idx_t k = 1;; ++k) {
        idx_t m = k * (k + 3) / 2;          /* 1‑based position of the '2' digit   */
        idx_t pos = m - 1;                  /* zero‑based index */
        if (pos >= (idx_t)need_s_digits) break;
        ++s_digits[pos];
    }

    /* prepare output array */
    uint8_t *out_digits = (uint8_t *)malloc(need_digits);
    if (!out_digits) { free(s_digits); return; }

    /* big integers for the digit‑by‑digit algorithm */
    BigInt rem, P, cand_base, test;
    bi_zero(&rem);
    bi_zero(&P);
    bi_zero(&cand_base);
    bi_zero(&test);

    /* first group (two decimal digits) */
    int g0 = s_digits[0] * 10 + s_digits[1];
    bi_set_uint(&rem, g0);

    for (size_t i = 0; i < need_digits; ++i) {
        /* compute base = P * 20 */
        bi_copy(&cand_base, &P);
        bi_mul_small(&cand_base, 20);

        int best_d = 0;
        BigInt best_test;
        bi_zero(&best_test);
        /* search for digit */
        for (int d = 9; d >= 0; --d) {
            bi_copy(&test, &cand_base);
            bi_add_small(&test, d);
            bi_mul_small(&test, d);
            if (bi_cmp(&test, &rem) <= 0) {
                best_d = d;
                bi_copy(&best_test, &test);
                break;
            }
        }

        /* update remainder */
        bi_sub(&rem, &best_test);

        /* record digit */
        out_digits[i] = (uint8_t)best_d;

        /* update P */
        bi_mul_small(&P, 10);
        bi_add_small(&P, best_d);

        /* bring next group if any */
        if (i + 1 < need_digits) {
            int gn = s_digits[2 * (i + 1)] * 10 + s_digits[2 * (i + 1) + 1];
            bi_mul_small(&rem, 100);
            bi_add_small(&rem, gn);
        }
    }

    /* replace old cache */
    free(cached_digits);
    cached_digits = out_digits;
    cached_max_n = n;

    free(s_digits);
}

seq_t a(idx_t n) {
    ensure_cached(n);
    return (seq_t)cached_digits[(size_t)n];
}
