/*
 * A010478  Decimal expansion of square root of 22.
 * https://oeis.org/A010478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = decimal digit number (n‑th digit) of sqrt(22)
 * Range:     u8 n <= 9590, u16 n <= 9590, u32 n <= 9590, u64 n <= 9590,
 *            u128 n <= 9590, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses a simple fixed‑size big integer type   */
/* that can handle arbitrarily many decimal digits.               */
/* It works for all seq_t types; for 64‑bit seq_t only the digit   */
/* (0‑9) is returned, which always fits in seq_t.                 */

#include <stdint.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/*   Fixed‑size big integer implementation (base 1e9)                */
/* ------------------------------------------------------------------ */
#define BIGINT_BASE 1000000000u        /* 1e9 */
#define BIGINT_MAX_LIMBS 2000          /* enough for 10000 decimal digits */

typedef struct {
    uint32_t d[BIGINT_MAX_LIMBS];
    size_t   len;                      /* number of limbs (0 == 0) */
} BigInt;

/* initialise to zero */
static void big_zero(BigInt *a) {
    a->len = 0;
}

/* load from unsigned 64‑bit integer */
static void big_from_u64(BigInt *a, uint64_t v) {
    a->len = 0;
    while (v) {
        a->d[a->len++] = v % BIGINT_BASE;
        v /= BIGINT_BASE;
    }
    if (a->len == 0) a->len = 0;   /* keep zero representation as len==0 */
}

/* copy */
static void big_copy(BigInt *dst, const BigInt *src) {
    dst->len = src->len;
    for (size_t i = 0; i < src->len; ++i) dst->d[i] = src->d[i];
}

/* compare a and b: return -1 if a<b, 0 if a==b, +1 if a>b */
static int big_cmp(const BigInt *a, const BigInt *b) {
    if (a->len != b->len) return (a->len < b->len) ? -1 : 1;
    for (size_t i = a->len; i-- > 0;) {
        if (a->d[i] != b->d[i])
            return (a->d[i] < b->d[i]) ? -1 : 1;
    }
    return 0;
}

/* multiply by small unsigned int (assumed < 128) */
static void big_mul_small(BigInt *a, uint32_t m) {
    if (a->len == 0 || m == 1) return;
    if (m == 0) { a->len = 0; return; }
    uint64_t carry = 0;
    for (size_t i = 0; i < a->len; ++i) {
        uint64_t prod = (uint64_t)a->d[i] * m + carry;
        a->d[i] = (uint32_t)(prod % BIGINT_BASE);
        carry  = prod / BIGINT_BASE;
    }
    if (carry) {
        a->d[a->len++] = (uint32_t)carry;
    }
}

/* add small unsigned int (assumed < 128) to a */
static void big_add_small(BigInt *a, uint32_t add) {
    uint64_t carry = add;
    for (size_t i = 0; i < a->len && carry; ++i) {
        uint64_t sum = (uint64_t)a->d[i] + carry;
        a->d[i] = (uint32_t)(sum % BIGINT_BASE);
        carry   = sum / BIGINT_BASE;
    }
    if (carry) {
        a->d[a->len++] = (uint32_t)carry;
    }
}

/* subtract small unsigned int (assumed ≤ a) from a */
static void big_sub_small(BigInt *a, uint32_t sub) {
    if (sub == 0) return;
    uint64_t borrow = sub;
    for (size_t i = 0; i < a->len; ++i) {
        uint64_t cur = (uint64_t)a->d[i];
        if (cur < borrow) {
            a->d[i] = (uint32_t)(cur + BIGINT_BASE - borrow);
            borrow = 1;
        } else {
            a->d[i] = (uint32_t)(cur - borrow);
            borrow  = 0;
            break;
        }
    }
    /* remove leading zeros */
    while (a->len > 0 && a->d[a->len - 1] == 0) --a->len;
}

/* subtract big integer b from a (assumes a >= b) */
static void big_sub_big(BigInt *a, const BigInt *b) {
    if (b->len == 0) return;
    uint64_t borrow = 0;
    for (size_t i = 0; i < a->len; ++i) {
        uint64_t cur = (uint64_t)(i < b->len ? b->d[i] : 0) + borrow;
        if (a->d[i] < cur) {
            a->d[i] = (uint32_t)(a->d[i] + BIGINT_BASE - cur);
            borrow  = 1;
        } else {
            a->d[i] = (uint32_t)(a->d[i] - cur);
            borrow  = 0;
        }
    }
    /* remove leading zeros */
    while (a->len > 0 && a->d[a->len - 1] == 0) --a->len;
}

/* ------------------------------------------------------------------ */
/*                     Main sequence function                          */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* static cache of digits */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t sz    = 0;

    /* state for the digit extraction */
    static BigInt p, r;      /* current root and remainder */
    static int initialized = 0;

    if (n == 0) return 0;    /* undefined, avoid UB */

    /* initialise on first call */
    if (!initialized) {
        big_from_u64(&p, 4);   /* sqrt(22) floor = 4 */
        big_from_u64(&r, 6);   /* 22 - 4^2 = 6 */
        cap = 64;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;  /* allocation failure */
        cache[0] = 4;          /* a(1) = 4 */
        sz = 1;
        initialized = 1;
    }

    /* grow cache if needed */
    if (sz < n) {
        if (cap < n) {
            size_t newcap = cap ? cap * 2 : 64;
            if (newcap < n) newcap = n;
            seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            if (!tmp) {
                free(cache);
                cache = NULL;
                cap = sz = 0;
                return 0;
            }
            cache = tmp;
            cap = newcap;
        }

        /* generate digits until we have at least n of them */
        while (sz < n) {
            /* bring down two zeroes: r *= 100 */
            big_mul_small(&r, 100);

            int digit = 0;
            BigInt cand;          /* candidate = (20*p + d) * d */

            for (int d = 9; d >= 0; --d) {
                /* cand = p * 20 + d */
                big_copy(&cand, &p);
                big_mul_small(&cand, 20);
                big_add_small(&cand, (uint32_t)d);

                /* cand = cand * d */
                big_mul_small(&cand, (uint32_t)d);

                if (big_cmp(&cand, &r) <= 0) {
                    digit = d;
                    break;
                }
            }

            /* update remainder */
            big_sub_big(&r, &cand);

            /* update root: p = p * 10 + digit */
            big_mul_small(&p, 10);
            big_add_small(&p, (uint32_t)digit);

            cache[sz++] = (seq_t)digit;
        }
    }

    return cache[n - 1];
}
