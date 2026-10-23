/*
 * A010492  Decimal expansion of square root of 38.
 * https://oeis.org/A010492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 9585, u16 n <= 9585, u32 n <= 9585, u64 n <= 9585,
 *            u128 n <= 9585, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: compute the nth decimal digit of √38 using long‑division
   digit extraction.  The first term a(1) is the integer part (6).  For
   n ≥ 2 the digits are produced by the standard digit‑by‑digit method. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* --- arbitrary‑precision integer over 10¹⁸ --- */
typedef struct {
    uint64_t *d;   /* least‑significant limb first, base = 10¹⁸ */
    size_t len;    /* number of limbs used */
} BigNum;

static const uint64_t BASE = 1000000000000000000ULL;  /* 10¹⁸ */

/* ------------------------------------------------------------------ */
/* basic constructors / destructors                                     */
static BigNum *big_create(void) {
    BigNum *bn = (BigNum*)malloc(sizeof(BigNum));
    if (!bn) return NULL;
    bn->d   = NULL;
    bn->len = 0;
    return bn;
}

static BigNum *big_from_uint64(uint64_t v) {
    if (v == 0) return big_create();
    BigNum *bn = big_create();
    if (!bn) return NULL;
    while (v) {
        bn->d = (uint64_t*)realloc(bn->d, (bn->len + 1) * sizeof(uint64_t));
        bn->d[bn->len++] = v % BASE;
        v /= BASE;
    }
    return bn;
}

static void big_free(BigNum *bn) {
    if (!bn) return;
    free(bn->d);
    free(bn);
}

/* ------------------------------------------------------------------ */
/* comparison                                                           */
static int big_cmp(const BigNum *a, const BigNum *b) {
    if (a->len != b->len) return (a->len < b->len) ? -1 : 1;
    for (size_t i = a->len; i-- > 0; ) {
        if (a->d[i] != b->d[i])
            return (a->d[i] < b->d[i]) ? -1 : 1;
    }
    return 0;
}

/* ------------------------------------------------------------------ */
/* multiplication by small integer (0 ≤ m ≤ 200 for this use)        */
static BigNum *big_mul_small(const BigNum *a, uint64_t m) {
    if (m == 0) return big_create(); /* zero */
    if (a->len == 0) return big_from_uint64(0);
    BigNum *res = big_create();
    if (!res) return NULL;
    res->d = (uint64_t*)malloc((a->len + 2) * sizeof(uint64_t));
    if (!res->d) { big_free(res); return NULL; }
    __uint128_t carry = 0;
    for (size_t i = 0; i < a->len; ++i) {
        __uint128_t prod = (__uint128_t)a->d[i] * m + carry;
        res->d[i] = (uint64_t)(prod % BASE);
        carry     = prod / BASE;
    }
    size_t idx = a->len;
    if (carry > 0) {
        res->d[idx++] = (uint64_t)carry;
    }
    res->len = idx;
    return res;
}

/* ------------------------------------------------------------------ */
/* addition of small integer (0 ≤ m ≤ 81 for this use)                 */
static BigNum *big_add_small(const BigNum *a, uint64_t m) {
    BigNum *res = big_create();
    if (!res) return NULL;
    res->d = (uint64_t*)malloc((a->len + 2) * sizeof(uint64_t));
    if (!res->d) { big_free(res); return NULL; }
    __uint128_t carry = m;
    size_t i = 0;
    for (; i < a->len; ++i) {
        __uint128_t sum = (__uint128_t)a->d[i] + carry;
        res->d[i] = (uint64_t)(sum % BASE);
        carry = sum / BASE;
    }
    if (carry > 0) {
        res->d[i++] = (uint64_t)carry;
    }
    res->len = i;
    return res;
}

/* ------------------------------------------------------------------ */
/* subtraction (assumes a >= b)                                        */
static BigNum *big_sub(const BigNum *a, const BigNum *b) {
    BigNum *res = big_create();
    if (!res) return NULL;
    res->d = (uint64_t*)malloc(a->len * sizeof(uint64_t));
    if (!res->d) { big_free(res); return NULL; }
    int64_t borrow = 0;
    for (size_t i = 0; i < a->len; ++i) {
        int64_t ai = (int64_t)a->d[i] - borrow - ((i < b->len) ? (int64_t)b->d[i] : 0);
        if (ai < 0) {
            ai += BASE;
            borrow = 1;
        } else
            borrow = 0;
        res->d[i] = (uint64_t)ai;
    }
    /* trim leading zeros */
    size_t len = a->len;
    while (len > 0 && res->d[len - 1] == 0) --len;
    res->len = len;
    if (len == 0) {
        free(res->d);
        res->d = NULL;
    }
    return res;
}

/* ------------------------------------------------------------------ */
/* public sequence function                                             */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* unused offset */
    if (n == 1) return (seq_t)6;          /* integer part */

    /* initial values: A = 6, R = 38 - 6² = 2 */
    BigNum *A = big_from_uint64(6);
    BigNum *R = big_from_uint64(2);

    /* generate digits 2 .. n */
    for (idx_t i = 2; i <= n; ++i) {
        /* bring down two zeros: R *= 100 */
        BigNum *tmp = big_mul_small(R, 100);
        big_free(R);
        R = tmp;

        /* compute 20*A once */
        BigNum *base20 = big_mul_small(A, 20);

        int chosen = 0;
        for (int d = 9; d >= 0; --d) {
            /* candidate = (base20 + d) * d = base20*d + d² */
            BigNum *prod = big_mul_small(base20, (uint64_t)d);
            /* add d*d (<=81) */
            BigNum *candidate = big_add_small(prod, (uint64_t)d * (uint64_t)d);
            big_free(prod);

            if (big_cmp(candidate, R) <= 0) {
                chosen = d;
                /* new R = R - candidate */
                BigNum *newR = big_sub(R, candidate);
                big_free(R);
                R = newR;
                big_free(candidate);
                break;
            }
            big_free(candidate);
        }
        big_free(base20);

        /* update A = A*10 + chosen */
        BigNum *mul10 = big_mul_small(A, 10);
        big_free(A);
        A = big_add_small(mul10, (uint64_t)chosen);
        big_free(mul10);

        if (i == n) {
            /* clean up */
            big_free(A);
            big_free(R);
            return (seq_t)chosen;
        }
    }

    /* should never reach here */
    big_free(A);
    big_free(R);
    return (seq_t)0;
}
