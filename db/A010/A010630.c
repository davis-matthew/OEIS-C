/*
 * A010630  Decimal expansion of cube root of 59.
 * https://oeis.org/A010630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the nth decimal digit (1‑based) of the cube
 * root of 59.  It generates the digits on demand using the
 * digit‑by‑digit algorithm for cubic roots.  The algorithm keeps an
 * internal state so that calls to a() in any order are fast. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Small big‑integer representation (base 1 000 000 000). */
#define BASE 1000000000U

typedef struct {
    size_t n;      /* number of limbs (little‑endian) */
    uint32_t *v;   /* limb array */
} BigInt;

/* ---------- local helper functions ---------- */

static BigInt *big_new(size_t n)
{
    BigInt *r = (BigInt*)malloc(sizeof(BigInt));
    r->n = n;
    r->v = n ? (uint32_t*)calloc(n, sizeof(uint32_t)) : NULL;
    return r;
}

static void big_free(BigInt *a)
{
    if (!a) return;
    free(a->v);
    free(a);
}

static BigInt *big_from_u64(uint64_t x)
{
    if (x == 0) return big_new(0);
    size_t n = 0;
    uint64_t t = x;
    while (t) { t /= BASE; n++; }
    BigInt *r = big_new(n);
    t = x;
    for (size_t i = 0; i < n; ++i) {
        r->v[i] = (uint32_t)(t % BASE);
        t /= BASE;
    }
    return r;
}

static int big_cmp(const BigInt *a, const BigInt *b)
{
    if (a->n > b->n) return 1;
    if (a->n < b->n) return -1;
    for (size_t i = a->n; i-- > 0;) {
        if (a->v[i] > b->v[i]) return 1;
        if (a->v[i] < b->v[i]) return -1;
    }
    return 0;
}

static BigInt *big_add(const BigInt *a, const BigInt *b)
{
    size_t m = a->n > b->n ? a->n : b->n;
    BigInt *r = big_new(m + 1);
    uint64_t carry = 0;
    size_t i;
    for (i = 0; i < m; ++i) {
        uint64_t av = (i < a->n) ? a->v[i] : 0;
        uint64_t bv = (i < b->n) ? b->v[i] : 0;
        uint64_t sum = av + bv + carry;
        r->v[i] = (uint32_t)(sum % BASE);
        carry = sum / BASE;
    }
    r->v[i] = (uint32_t)carry;
    r->n = m + (carry ? 1 : 0);
    return r;
}

static BigInt *big_sub(const BigInt *a, const BigInt *b) /* a >= b */
{
    BigInt *r = big_new(a->n);
    int64_t borrow = 0;
    size_t i;
    for (i = 0; i < a->n; ++i) {
        int64_t av = a->v[i];
        int64_t bv = (i < b->n) ? b->v[i] : 0;
        int64_t diff = av - bv - borrow;
        if (diff < 0) { diff += BASE; borrow = 1; }
        else { borrow = 0; }
        r->v[i] = (uint32_t)diff;
    }
    while (r->n > 0 && r->v[r->n-1] == 0) r->n--;
    return r;
}

static BigInt *big_mul_small(const BigInt *a, uint32_t m)
{
    if (m == 0 || a->n == 0) return big_new(0);
    BigInt *r = big_new(a->n + 1);
    uint64_t carry = 0;
    size_t i;
    for (i = 0; i < a->n; ++i) {
        uint64_t prod = (uint64_t)a->v[i] * m + carry;
        r->v[i] = (uint32_t)(prod % BASE);
        carry = prod / BASE;
    }
    r->v[i] = (uint32_t)carry;
    r->n = a->n + (carry ? 1 : 0);
    return r;
}

static void big_add_small(BigInt *a, uint32_t s)  /* modifies a in place */
{
    uint64_t sum = (uint64_t)a->v[0] + s;
    a->v[0] = (uint32_t)(sum % BASE);
    uint64_t carry = sum / BASE;
    size_t i = 1;
    while (carry) {
        if (i >= a->n) {
            a->v = (uint32_t*)realloc(a->v, (a->n + 1) * sizeof(uint32_t));
            a->v[a->n] = 0;
            a->n++;
        }
        uint64_t v = (uint64_t)a->v[i] + carry;
        a->v[i] = (uint32_t)(v % BASE);
        carry = v / BASE;
        i++;
    }
}

/* ---------- cache and internal state ---------- */

static uint8_t *digit_cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

static BigInt *P  = NULL; /* current integer part (as an integer) */
static BigInt *P2 = NULL; /* P^2 */
static BigInt *R  = NULL; /* remainder N - P^3 */

/* Ensure cache contains at least n digits */
static void cache_ensure(size_t n)
{
    if (n <= cache_len) return;

    /* grow cache storage if necessary */
    if (n > cache_cap) {
        size_t new_cap = (cache_cap > 0) ? cache_cap * 2 : 64;
        if (new_cap < n) new_cap = n;
        uint8_t *tmp = (uint8_t*)realloc(digit_cache, new_cap * sizeof(uint8_t));
        if (!tmp) exit(EXIT_FAILURE);
        digit_cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise state on first call */
    if (!P) {
        P  = big_from_u64(3);          /* integer part */
        P2 = big_from_u64(9);          /* 3^2 */
        /* R = 59 - 3^3 = 32 */
        BigInt *tmp = big_from_u64(59);
        BigInt *tmp2 = big_from_u64(27);  /* 3^3 */
        R = big_sub(tmp, tmp2);
        big_free(tmp); big_free(tmp2);
        digit_cache[0] = 3;
        cache_len = 1;
    }

    /* generate further digits as needed */
    while (cache_len < n) {
        /* R1000 = R * 1000 */
        BigInt *R1000 = big_mul_small(R, 1000);
        /* limit = 300 * P2 (i.e. 300 * P^2) */
        BigInt *limit = big_mul_small(P2, 300);

        int chosen = -1;
        uint8_t digit = 0;
        BigInt *sum = NULL;  /* will hold (300P^2 d + 30P d^2 + d^3) */

        for (int d = 9; d >= 0; --d) {
            BigInt *tmp1 = big_mul_small(limit, (uint32_t)d);   /* 300*P2*d */
            uint32_t factor = 30u * d * d;                      /* 30*d^2 */
            BigInt *tmp2 = big_mul_small(P, factor);           /* 30*P*d^2 */
            sum = big_add(tmp1, tmp2);                         /* tmp1 + tmp2 */
            big_free(tmp1); big_free(tmp2);

            big_add_small(sum, (uint32_t)(d * d * d));          /* + d^3 */

            if (big_cmp(sum, R1000) <= 0) {                   /* eligible digit */
                chosen = d;
                digit = (uint8_t)d;
                break;
            }
            big_free(sum);
            sum = NULL;
        }

        if (chosen < 0) {
            /* Should never happen for cube root of 59 */
            exit(EXIT_FAILURE);
        }

        /* Update remainder: R = R1000 - sum */
        BigInt *newR = big_sub(R1000, sum);
        big_free(R);
        big_free(R1000);
        big_free(sum);
        R = newR;

        /* Update P2: (10*P + d)^2 = 100*P^2 + 20*P*d + d^2 */
        BigInt *tmpA = big_mul_small(P2, 100);                 /* 100*oldP2 */
        BigInt *tmpB = big_mul_small(P, 20u * (uint32_t)digit);/* 20*oldP*d */
        BigInt *tmpC = big_from_u64((uint64_t)digit * digit);  /* d^2 */
        BigInt *tmpD = big_add(tmpA, tmpB);
        big_free(tmpA); big_free(tmpB);
        BigInt *newP2 = big_add(tmpD, tmpC);
        big_free(tmpD); big_free(tmpC);
        big_free(P2);
        P2 = newP2;

        /* Update P: P = 10*oldP + d */
        BigInt *newP = big_mul_small(P, 10);
        big_add_small(newP, digit);
        big_free(P);
        P = newP;

        /* Cache the digit */
        digit_cache[cache_len++] = digit;
    }
}

/* ---------- exported sequence function ---------- */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* out of range, undefined */
    cache_ensure((size_t)n);
    return (seq_t)(digit_cache[n-1]);
}
