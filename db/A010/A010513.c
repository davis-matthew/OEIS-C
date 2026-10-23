/*
 * A010513  Decimal expansion of square root of 60.
 * https://oeis.org/A010513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons
 * Formula:   A010513 – decimal expansion of sqrt(60)
 * Range:     u8 n <= 9595, u16 n <= 9595, u32 n <= 9595, u64 n <= 9595,
 *            u128 n <= 9595, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

#define BASE 1000000000U
#define MAX_LIMBS 1200   /* enough for 10000 decimal digits (~1112 limbs) */

typedef struct {
    uint32_t d[MAX_LIMBS];
    size_t sz;
} BI;

/* ---------- Big integer helpers ---------- */
static void bi_normalize(BI *a) {
    while (a->sz > 0 && a->d[a->sz - 1] == 0) a->sz--;
}

static void bi_from_uint(BI *a, uint64_t val) {
    a->sz = 0;
    while (val) {
        a->d[a->sz++] = (uint32_t)(val % BASE);
        val /= BASE;
    }
    bi_normalize(a);
}

static void bi_copy(BI *dst, const BI *src) {
    dst->sz = src->sz;
    for (size_t i = 0; i < src->sz; ++i) dst->d[i] = src->d[i];
}

static int bi_cmp(const BI *a, const BI *b) {
    if (a->sz != b->sz) return (a->sz > b->sz) ? 1 : -1;
    for (size_t i = a->sz; i-- > 0; ) {
        if (a->d[i] != b->d[i]) return (a->d[i] > b->d[i]) ? 1 : -1;
    }
    return 0;
}

static void bi_add_small(BI *a, uint32_t val) {
    size_t i = 0;
    uint64_t sum = (uint64_t)a->d[0] + val;
    a->d[0] = (uint32_t)(sum % BASE);
    uint32_t carry = (uint32_t)(sum / BASE);
    i = 1;
    while (carry && i < a->sz) {
        sum = (uint64_t)a->d[i] + carry;
        a->d[i] = (uint32_t)(sum % BASE);
        carry = (uint32_t)(sum / BASE);
        ++i;
    }
    if (carry) {
        a->d[a->sz++] = carry;
    }
}

static void bi_mul_small(BI *a, uint32_t val) {
    uint64_t carry = 0;
    for (size_t i = 0; i < a->sz; ++i) {
        uint64_t prod = (uint64_t)a->d[i] * val + carry;
        a->d[i] = (uint32_t)(prod % BASE);
        carry = prod / BASE;
    }
    if (carry) {
        a->d[a->sz++] = (uint32_t)carry;
    }
    bi_normalize(a);
}

static void bi_sub(BI *a, const BI *b) {
    uint64_t borrow = 0;
    for (size_t i = 0; i < a->sz; ++i) {
        uint64_t sub = (uint64_t)a->d[i] - ((i < b->sz) ? b->d[i] : 0) - borrow;
        if (sub > (uint64_t)0xFFFFFFFF) { /* borrow */
            a->d[i] = (uint32_t)(sub + BASE);
            borrow = 1;
        } else {
            a->d[i] = (uint32_t)sub;
            borrow = 0;
        }
    }
    bi_normalize(a);
}
/* ---------------------------------------- */

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc++98-compat"   /* for static global initialization */

static seq_t *digit_cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static int initialized = 0;

/* state of the digit calculation */
static BI S_p;   /* current root (integer part + digits so far) */
static BI S_rem; /* current remainder */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined offset; harmless */

    /* initialise on first call */
    if (!initialized) {
        /* allocate cache */
        cache_cap = 128;
        digit_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;

        /* integer part */
        bi_from_uint(&S_p, 7);
        bi_from_uint(&S_rem, 60);
        /* rem = 60 - 49 = 11 */
        BI temp;
        bi_from_uint(&temp, 49);
        bi_sub(&S_rem, &temp);

        /* cache first digit (integer part) */
        if (cache_len == cache_cap) {
            cache_cap *= 2;
            digit_cache = (seq_t *)realloc(digit_cache, cache_cap * sizeof(seq_t));
        }
        digit_cache[cache_len++] = 7;

        initialized = 1;
    }

    /* generate digits until we have at least n of them */
    while (cache_len < (size_t)n) {
        /* bring down next two decimal digits */
        bi_mul_small(&S_rem, 100);

        /* factor = p * 20 */
        BI factor;
        bi_copy(&factor, &S_p);
        bi_mul_small(&factor, 20);

        int chosen_digit = 0;
        for (int d = 9; d >= 0; --d) {
            BI candidate;
            bi_copy(&candidate, &factor);
            bi_add_small(&candidate, (uint32_t)d);
            bi_mul_small(&candidate, (uint32_t)d);
            if (bi_cmp(&candidate, &S_rem) <= 0) {
                chosen_digit = d;
                break;
            }
        }

        /* recompute candidate for the chosen digit to subtract */
        BI cand;
        bi_copy(&cand, &factor);
        bi_add_small(&cand, (uint32_t)chosen_digit);
        bi_mul_small(&cand, (uint32_t)chosen_digit);
        bi_sub(&S_rem, &cand);

        /* update p = p * 10 + d */
        bi_mul_small(&S_p, 10);
        if (chosen_digit) bi_add_small(&S_p, (uint32_t)chosen_digit);

        /* cache the digit */
        if (cache_len == cache_cap) {
            cache_cap *= 2;
            digit_cache = (seq_t *)realloc(digit_cache, cache_cap * sizeof(seq_t));
        }
        digit_cache[cache_len++] = (seq_t)chosen_digit;
    }

    return digit_cache[n - 1];
}
#pragma clang diagnostic pop
