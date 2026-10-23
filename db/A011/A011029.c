/*
 * A011029  Decimal expansion of 4th root of 34.
 * https://oeis.org/A011029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = digit n of the decimal expansion of 34^(1/4) * (first
 *            term a(1) = 2, a(2) = 4, …). Computation is performed * by
 *            digit‑by‑digit extraction of the 4th root using big integers. *
 *            Only integer arithmetic on base 10^9 is used
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wstrict-prototypes"

/* ------------------------------------------------------------------ */
/* Big integer in base 10^9, little endian.                            */
#define BI_BASE 1000000000u
#define BI_MAX_DIG 5000          /* sufficient for up to 10000 digits */

typedef struct {
    uint32_t d[BI_MAX_DIG];
    size_t len;
} BigInt;

/* ------------------------------------------------------------------ */
/* Basic operations on BigInt                                          */

static inline void bi_set_zero(BigInt *a) { a->len = 0; }
static inline void bi_set_uint(BigInt *a, uint32_t v) {
    if (v == 0) { a->len = 0; return; }
    a->len = 1; a->d[0] = v;
}
static inline void bi_copy(BigInt *dst, const BigInt *src) {
    dst->len = src->len;
    memcpy(dst->d, src->d, src->len * sizeof(uint32_t));
}
static inline void bi_normalize(BigInt *a) {
    while (a->len && a->d[a->len-1] == 0) a->len--;
}
static inline int bi_cmp(const BigInt *a, const BigInt *b) {
    if (a->len != b->len) return (a->len > b->len) ? 1 : -1;
    for (size_t i = a->len; i-- > 0; ) {
        if (a->d[i] != b->d[i]) return (a->d[i] > b->d[i]) ? 1 : -1;
    }
    return 0;
}
static inline void bi_add(BigInt *a, const BigInt *b) {
    size_t n = a->len > b->len ? a->len : b->len;
    uint64_t carry = 0;
    for (size_t i = 0; i < n || carry; ++i) {
        uint64_t sum = carry;
        if (i < a->len) sum += a->d[i];
        if (i < b->len) sum += b->d[i];
        if (i < a->len) a->d[i] = (uint32_t)(sum % BI_BASE);
        else a->d[i] = (uint32_t)(sum % BI_BASE);
        carry = sum / BI_BASE;
    }
    a->len = n + (carry ? 1 : 0);
}
static inline void bi_add_small(BigInt *a, uint32_t v) {
    uint64_t carry = v;
    for (size_t i = 0; i < a->len && carry; ++i) {
        uint64_t sum = (uint64_t)a->d[i] + carry;
        a->d[i] = (uint32_t)(sum % BI_BASE);
        carry = sum / BI_BASE;
    }
    if (carry) {
        a->d[a->len++] = (uint32_t)carry;
    }
}
static inline void bi_mul_small(BigInt *a, uint32_t m) {
    if (m == 0 || a->len == 0) { a->len = 0; return; }
    uint64_t carry = 0;
    for (size_t i = 0; i < a->len; ++i) {
        uint64_t prod = (uint64_t)a->d[i] * m + carry;
        a->d[i] = (uint32_t)(prod % BI_BASE);
        carry = prod / BI_BASE;
    }
    while (carry) {
        a->d[a->len++] = (uint32_t)(carry % BI_BASE);
        carry /= BI_BASE;
    }
}
static inline void bi_sub(BigInt *a, const BigInt *b) /* assumes a >= b */ {
    uint64_t borrow = 0;
    for (size_t i = 0; i < a->len; ++i) {
        uint64_t sub = (uint64_t)a->d[i] - borrow - (i < b->len ? b->d[i] : 0);
        if ((int64_t)sub < 0) {
            sub += BI_BASE;
            borrow = 1;
        } else borrow = 0;
        a->d[i] = (uint32_t)sub;
    }
    bi_normalize(a);
}

/* ------------------------------------------------------------------ */
/* Compute the 4th‑root digit extraction helper.
 * f = 4000*p3*d + 600*p2*d^2 + 40*p*d^3 + d^4
 */
static void compute_f(const BigInt *p, const BigInt *p2, const BigInt *p3,
                      uint32_t d, BigInt *f)
{
    bi_set_zero(f);
    if (d == 0) { /* f stays 0 */ return; }

    uint32_t coeff1 = 4000 * d;          /* 4000*d  <= 4000*9 = 36000 */
    uint32_t coeff2 = 600 * d * d;       /* <= 600*81 = 48600 */
    uint32_t coeff3 = 40  * d * d * d;    /* <= 40*729 = 29160 */
    uint32_t d4  = d*d*d*d;              /* <= 6561 */

    BigInt tmp;

    /* (4000*d)*p3 */
    bi_copy(&tmp, p3);
    bi_mul_small(&tmp, coeff1);
    bi_add(f, &tmp);

    /* (600*d^2)*p2 */
    bi_copy(&tmp, p2);
    bi_mul_small(&tmp, coeff2);
    bi_add(f, &tmp);

    /* (40*d^3)*p */
    bi_copy(&tmp, p);
    bi_mul_small(&tmp, coeff3);
    bi_add(f, &tmp);

    /* d^4 */
    bi_add_small(f, d4);
}

/* ------------------------------------------------------------------ */
/* Generate digits up to a given n (1‑based). */
static void generate_upto(idx_t n, seq_t *cache, size_t *cache_cap, idx_t *computed,
                          BigInt *p, BigInt *p2, BigInt *p3, BigInt *rem)
{
    static bool initialized = false;
    if (!initialized) { /* just for safety, but not strictly needed */
        initialized = true;
    }

    for (idx_t k = *computed + 1; k <= n; ++k) {
        /* bring down next 4‑digit block */
        bi_mul_small(rem, 10000u);
        if (k == 1) bi_add_small(rem, 34u);  /* first group is 34 */

        /* find largest digit d such that f <= rem */
        uint32_t chosen_d = 0;
        for (int d = 9; d >= 0; --d) {
            BigInt f;
            compute_f(p, p2, p3, (uint32_t)d, &f);
            if (bi_cmp(&f, rem) <= 0) {
                chosen_d = (uint32_t)d;
                /* subtract f from remainder */
                bi_sub(rem, &f);
                break;
            }
        }

        /* update root and its powers */
        BigInt newp, newp2, newp3, tmp;

        /* newp = p*10 + d */
        bi_copy(&newp, p);
        bi_mul_small(&newp, 10u);
        bi_add_small(&newp, chosen_d);

        /* newp2 = p2*100 + p*20*d + d^2 */
        bi_copy(&newp2, p2);
        bi_mul_small(&newp2, 100u);
        bi_copy(&tmp, p);
        bi_mul_small(&tmp, 20u * chosen_d);
        bi_add(&newp2, &tmp);
        bi_add_small(&newp2, chosen_d * chosen_d);

        /* newp3 = p3*1000 + p2*300*d + p*30*d^2 + d^3 */
        bi_copy(&newp3, p3);
        bi_mul_small(&newp3, 1000u);
        bi_copy(&tmp, p2);
        bi_mul_small(&tmp, 300u * chosen_d);
        bi_add(&newp3, &tmp);
        bi_copy(&tmp, p);
        bi_mul_small(&tmp, 30u * chosen_d * chosen_d);
        bi_add(&newp3, &tmp);
        bi_add_small(&newp3, chosen_d * chosen_d * chosen_d);

        /* assign new values */
        bi_copy(p,  &newp);
        bi_copy(p2, &newp2);
        bi_copy(p3, &newp3);

        /* store last decimal digit of current root */
        uint32_t last_digit = (p->len ? (p->d[0] % 10u) : 0u);
        cache[k] = (seq_t)last_digit;
    }

    *computed = n;
}

/* ------------------------------------------------------------------ */
/* Main sequence function. hard-coded external linkage. */
seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *digit_cache = NULL;
    static size_t cache_cap   = 0;
    static idx_t cached_len   = 0;

    /* big integers for state */
    static BigInt p, p2, p3, rem;
    /* initialise once */
    static bool init_done = false;
    if (!init_done) {
        bi_set_zero(&p);  bi_set_zero(&p2);  bi_set_zero(&p3);  bi_set_zero(&rem);
        init_done = true;
    }

    if (n + 1 > cache_cap) {  /* allocate or grow cache */
        size_t new_cap = (n + 1) * 2;
        seq_t *tmp = (seq_t *)realloc(digit_cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* memory failure, return 0 */
        digit_cache = tmp;
        cache_cap   = new_cap;
    }

    if (n > cached_len) {
        generate_upto(n, digit_cache, &cache_cap, &cached_len,
                     &p, &p2, &p3, &rem);
    }
    return digit_cache[n];
}
#pragma clang diagnostic pop
