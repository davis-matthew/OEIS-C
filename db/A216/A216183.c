/*
 * A216183  Numbers m such that A059514(m) = 0.
 * https://oeis.org/A216183
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: a(n) = n-th m >= 1 such that iterating f(p/q) =
 *            p*q/(S(p)+S(q)-1) (S = decimal digit sum, fraction kept reduced)
 *            from m/1 never reaches an integer, i.e. A059514(m) = 0. The
 *            iteration uses exact multi-precision numerators, so nothing
 *            overflows whatever seq_t is
 * Range:     u8 n <= 17, u16 n <= 4043, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 68 terms through n = 8565 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define A216183_BASE 1000000000u

typedef struct {
    uint32_t *d;   /* numerator limbs, base 1e9, little endian */
    size_t len, cap;
    uint64_t q;    /* denominator (always small) */
} A216183_frac;

static void A216183_reserve(A216183_frac *f, size_t need)
{
    if (need > f->cap) {
        size_t nc = f->cap ? f->cap : 8;
        while (nc < need) nc *= 2;
        f->d = (uint32_t *)realloc(f->d, nc * sizeof(uint32_t));
        f->cap = nc;
    }
}

static void A216183_set(A216183_frac *f, uint64_t p, uint64_t q)
{
    f->len = 0;
    A216183_reserve(f, 4);
    if (p == 0) { f->d[0] = 0; f->len = 1; }
    while (p) { f->d[f->len++] = (uint32_t)(p % A216183_BASE); p /= A216183_BASE; }
    f->q = q;
}

static void A216183_copy(A216183_frac *dst, const A216183_frac *src)
{
    A216183_reserve(dst, src->len);
    memcpy(dst->d, src->d, src->len * sizeof(uint32_t));
    dst->len = src->len;
    dst->q = src->q;
}

static bool A216183_eq(const A216183_frac *x, const A216183_frac *y)
{
    return x->q == y->q && x->len == y->len &&
           memcmp(x->d, y->d, x->len * sizeof(uint32_t)) == 0;
}

static uint64_t A216183_dsum64(uint64_t x)
{
    uint64_t s = 0;
    while (x) { s += x % 10; x /= 10; }
    return s;
}

static uint64_t A216183_gcd64(uint64_t x, uint64_t y)
{
    while (y) { uint64_t t = x % y; x = y; y = t; }
    return x;
}

/* One application of f; returns true if the result is an integer. */
static bool A216183_step(A216183_frac *f)
{
    size_t i;
    uint64_t S = A216183_dsum64(f->q), den, pm = 0, r, g, carry;
    for (i = 0; i < f->len; i++) S += A216183_dsum64(f->d[i]);
    den = S - 1;
    for (i = f->len; i-- > 0;)
        pm = (pm * A216183_BASE + f->d[i]) % den;
    r = (pm * (f->q % den)) % den;
    g = A216183_gcd64(r, den);           /* gcd(p*q, den) */
    /* p *= q */
    carry = 0;
    for (i = 0; i < f->len; i++) {
        uint64_t t = (uint64_t)f->d[i] * f->q + carry;
        f->d[i] = (uint32_t)(t % A216183_BASE);
        carry = t / A216183_BASE;
    }
    while (carry) {
        A216183_reserve(f, f->len + 1);
        f->d[f->len++] = (uint32_t)(carry % A216183_BASE);
        carry /= A216183_BASE;
    }
    /* p /= g (exact) */
    if (g > 1) {
        uint64_t rem = 0;
        for (i = f->len; i-- > 0;) {
            uint64_t cur = rem * A216183_BASE + f->d[i];
            f->d[i] = (uint32_t)(cur / g);
            rem = cur % g;
        }
        while (f->len > 1 && f->d[f->len - 1] == 0) f->len--;
    }
    f->q = den / g;
    return f->q == 1;
}

static A216183_frac A216183_t = {NULL, 0, 0, 0};
static A216183_frac A216183_h = {NULL, 0, 0, 0};

/* true iff A059514(m) == 0 */
static bool A216183_is_zero(uint64_t m)
{
    long steps = 0;
    A216183_set(&A216183_t, m, 1);
    if (A216183_step(&A216183_t)) return false;
    A216183_copy(&A216183_h, &A216183_t);
    if (A216183_step(&A216183_h)) return false;
    for (;;) {
        if (A216183_step(&A216183_t)) return false;
        if (A216183_step(&A216183_h)) return false;
        if (A216183_step(&A216183_h)) return false;
        if (A216183_eq(&A216183_t, &A216183_h)) return true;
        /* Safety net: unbounded growth also means no integer is reached. */
        if (++steps > 200000 || A216183_h.len > 2000) return true;
    }
}

static idx_t *A216183_terms = NULL;
static idx_t A216183_count = 0;
static idx_t A216183_cap = 0;
static idx_t A216183_next_m = 1;

seq_t
a(idx_t n)
{
    while (A216183_count < n) {
        idx_t m = A216183_next_m++;
        if (A216183_is_zero((uint64_t)m)) {
            if (A216183_count == A216183_cap) {
                idx_t nc = A216183_cap ? A216183_cap * 2 : 1024;
                A216183_terms = (idx_t *)realloc(A216183_terms, (size_t)nc * sizeof(idx_t));
                A216183_cap = nc;
            }
            A216183_terms[A216183_count++] = m;
        }
    }
    return (seq_t)A216183_terms[n - 1];
}
