/*
 * A200991  Decimal expansion of square root of 221/25.
 * https://oeis.org/A200991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons
 * Formula:   decimal digits of √(221/25) = √(8.84)
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm uses the digit‑by‑digit square‑root extraction
 * with arbitrary‑precision integers implemented in base 1e9.
 * It maintains the current root and remainder as big integers
 * and produces digits in O(n) time. */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

/* -------------------------------------------------------------------------- */
/* Big integer type: base 10^9, little‑endian digits                             */
/* -------------------------------------------------------------------------- */
#define BIG_BASE      1000000000U      /* 1e9 */
#define BIG_MAX_LIMBS 2000              /* > 10k decimal digits / 9 ≈ 1112 */

typedef struct {
    uint32_t d[BIG_MAX_LIMBS];
    size_t  len;                      /* number of used limbs (least significant first) */
} BigInt;

/* Zero a big integer */
static void bi_zero(BigInt *a)
{
    a->len = 0;
}

/* Copy a big integer */
static void bi_copy(BigInt *dst, const BigInt *src)
{
    dst->len = src->len;
    if (src->len) memcpy(dst->d, src->d, src->len * sizeof(uint32_t));
}

/* Multiply by small integer <= 100 */
static void bi_mul_small(BigInt *a, uint32_t m)
{
    if (m == 0) { bi_zero(a); return; }
    uint64_t carry = 0;
    for (size_t i = 0; i < a->len; ++i) {
        uint64_t prod = (uint64_t)a->d[i] * m + carry;
        a->d[i] = (uint32_t)(prod % BIG_BASE);
        carry   = prod / BIG_BASE;
    }
    if (carry) {
        a->d[a->len++] = (uint32_t)carry;
    }
}

/* Add small integer (0 <= v < BIG_BASE) */
static void bi_add_small(BigInt *a, uint32_t v)
{
    uint64_t sum = (uint64_t)(a->len ? a->d[0] : 0) + v;
    if (a->len == 0) {
        a->d[0] = (uint32_t)(sum % BIG_BASE);
        a->len = 1;
        if (sum >= BIG_BASE) a->d[1] = 1, a->len = 2;
        return;
    }
    a->d[0] = (uint32_t)(sum % BIG_BASE);
    uint64_t carry = sum / BIG_BASE;
    size_t i = 1;
    while (carry && i < a->len) {
        uint64_t s = (uint64_t)a->d[i] + carry;
        a->d[i] = (uint32_t)(s % BIG_BASE);
        carry   = s / BIG_BASE;
        ++i;
    }
    if (carry) {
        a->d[a->len++] = (uint32_t)carry;
    }
}

/* Subtract b from a (a >= b) */
static void bi_sub(BigInt *a, const BigInt *b)
{
    int64_t carry = 0;
    for (size_t i = 0; i < a->len; ++i) {
        int64_t diff = (int64_t)a->d[i] -
                       (i < b->len ? (int64_t)b->d[i] : 0) +
                       carry;
        if (diff < 0) {
            diff += BIG_BASE;
            carry = -1;
        } else {
            carry = 0;
        }
        a->d[i] = (uint32_t)diff;
    }
    while (a->len > 0 && a->len > 0 && a->d[a->len - 1] == 0) --a->len;
}

/* Compare two big integers: returns -1, 0, 1 for <, ==, > */
static int bi_cmp(const BigInt *a, const BigInt *b)
{
    if (a->len != b->len) return (a->len > b->len) ? 1 : -1;
    for (size_t i = a->len; i > 0; --i) {
        if (a->d[i - 1] != b->d[i - 1])
            return (a->d[i - 1] > b->d[i - 1]) ? 1 : -1;
    }
    return 0;
}

/* -------------------------------------------------------------------------- */
/* Function to compute the n‑th decimal digit of √(221/25)                 */
/* -------------------------------------------------------------------------- */
static BigInt root;  /* current root value (integer part × 10^k) */
static BigInt rem;   /* current remainder */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

/* Helper to ensure cache capacity */
static void cache_grow(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!p) { /* memory failure, abort silently */
        exit(1);
    }
    cache = p;
    cache_cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset not used */

    size_t target = (size_t)n;
    cache_grow(target);
    while (cache_len < target) {
        /* Determine next 2‑digit group */
        int group = (cache_len == 0) ? 8
                     : (cache_len == 1) ? 84
                     : 0;

        /* rem = rem * 100 + group */
        bi_mul_small(&rem, 100);
        bi_add_small(&rem, (uint32_t)group);

        /* divisor = 20 * root */
        BigInt divisor;
        bi_copy(&divisor, &root);
        bi_mul_small(&divisor, 20);

        /* Find largest d : (divisor + d) * d <= rem */
        int best_d = 0;
        for (int d = 9; d >= 0; --d) {
            BigInt tmp;
            bi_copy(&tmp, &divisor);
            bi_add_small(&tmp, (uint32_t)d);

            BigInt cand;
            bi_copy(&cand, &tmp);
            bi_mul_small(&cand, (uint32_t)d);

            if (bi_cmp(&cand, &rem) <= 0) {
                /* Subtract cand from remainder */
                bi_sub(&rem, &cand);
                /* Update root: root = root * 10 + d */
                bi_mul_small(&root, 10);
                bi_add_small(&root, (uint32_t)d);
                best_d = d;
                break;
            }
        }
        cache[cache_len++] = (seq_t)best_d;
    }
    return cache[target - 1];
}
