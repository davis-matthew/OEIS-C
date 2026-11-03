/*
 * A010523  Decimal expansion of square root of 71.
 * https://oeis.org/A010523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 20000, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 115 terms through n = 9320 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A010523: the decimal digits of sqrt(71), a(1) being the first.
 *
 * Digit-by-digit square root in base 10^9 limbs that grow as needed (the
 * original had a fixed number of limbs and overran it near 18000 digits):
 * with p the root so far and r = 71 10^(2j) - p^2, each step sets r = 100 r and
 * takes the largest d with (20 p + d) d <= r. d is estimated from the leading
 * limbs and corrected, so a digit costs time linear in the length. The state
 * is kept, so asking for more digits continues where the last call stopped. */

#include <stdint.h>
#include <stdlib.h>

#define K 71
#define OFF 1                         /* index of the first digit */

typedef struct { uint32_t *d; size_t len, cap; } num;   /* little-endian base 10^9 */

static void reserve(num *x, size_t n)
{
    if (n <= x->cap) return;
    size_t c = x->cap ? x->cap : 16;
    while (c < n) c *= 2;
    uint32_t *t = (uint32_t *)realloc(x->d, c * sizeof *t);
    if (!t) abort();
    x->d = t;
    x->cap = c;
}

static void trim(num *x)
{
    while (x->len && x->d[x->len - 1] == 0) --x->len;
}

static void mul_small_add(num *x, uint32_t m, uint32_t add)    /* x = x m + add */
{
    uint64_t c = add;
    for (size_t i = 0; i < x->len; ++i) {
        c += (uint64_t)x->d[i] * m;
        x->d[i] = (uint32_t)(c % 1000000000u);
        c /= 1000000000u;
    }
    while (c) {
        reserve(x, x->len + 1);
        x->d[x->len++] = (uint32_t)(c % 1000000000u);
        c /= 1000000000u;
    }
    trim(x);
}

static int cmp(const num *x, const num *y)
{
    if (x->len != y->len) return x->len < y->len ? -1 : 1;
    for (size_t i = x->len; i-- > 0;)
        if (x->d[i] != y->d[i]) return x->d[i] < y->d[i] ? -1 : 1;
    return 0;
}

static void sub(num *x, const num *y)                         /* x -= y, x >= y */
{
    int64_t b = 0;
    for (size_t i = 0; i < x->len; ++i) {
        int64_t v = (int64_t)x->d[i] - (i < y->len ? y->d[i] : 0) - b;
        b = v < 0;
        x->d[i] = (uint32_t)(b ? v + 1000000000 : v);
    }
    trim(x);
}

static double lead(const num *x, size_t len)  /* x / 10^(9 (len - 3)), roughly */
{
    double v = 0;
    for (size_t i = len; i-- > 0 && i + 3 >= len;) v = v * 1e9 + (i < x->len ? x->d[i] : 0);
    return v;
}

static num P, Rm, T;                        /* root, remainder, scratch */
static unsigned char *digits = NULL;
static size_t ndig = 0, dcap = 0;

static void next_digit(void)
{
    mul_small_add(&Rm, 100, 0);
    /* estimate d = r / (20 p) from the leading limbs, then correct */
    size_t L = Rm.len > P.len + 1 ? Rm.len : P.len + 1;
    double est = P.len ? lead(&Rm, L) / (20.0 * lead(&P, L)) : 9.0;
    int d = est >= 9.0 ? 9 : est <= 0 ? 0 : (int)est;
    for (;; --d) {
        /* T = (20 p + d) d */
        reserve(&T, P.len + 2);
        T.len = P.len;
        for (size_t i = 0; i < P.len; ++i) T.d[i] = P.d[i];
        mul_small_add(&T, 20, (uint32_t)d);
        mul_small_add(&T, (uint32_t)d, 0);
        if (cmp(&T, &Rm) <= 0 || d == 0) break;
    }
    for (;;) {                               /* the estimate may also be low */
        num U = {0, 0, 0};
        reserve(&U, P.len + 2);
        U.len = P.len;
        for (size_t i = 0; i < P.len; ++i) U.d[i] = P.d[i];
        mul_small_add(&U, 20, (uint32_t)(d + 1));
        mul_small_add(&U, (uint32_t)(d + 1), 0);
        int up = d < 9 && cmp(&U, &Rm) <= 0;
        if (up) {
            reserve(&T, U.len);
            T.len = U.len;
            for (size_t i = 0; i < U.len; ++i) T.d[i] = U.d[i];
            ++d;
        }
        free(U.d);
        if (!up) break;
    }
    sub(&Rm, &T);
    mul_small_add(&P, 10, (uint32_t)d);
    if (ndig == dcap) {
        dcap = dcap ? 2 * dcap : 1024;
        unsigned char *t = (unsigned char *)realloc(digits, dcap);
        if (!t) abort();
        digits = t;
    }
    digits[ndig++] = (unsigned char)d;
}

seq_t a(idx_t n)
{
    if (n < OFF) return (seq_t)0;
    if (ndig == 0) {                         /* the integer part: K < 100, one digit */
        unsigned s = 0;
        while ((s + 1) * (s + 1) <= K) ++s;
        reserve(&P, 1);
        P.d[0] = s;
        P.len = s ? 1 : 0;
        reserve(&Rm, 1);
        Rm.d[0] = K - s * s;
        Rm.len = Rm.d[0] ? 1 : 0;
        reserve(&T, 1);
        digits = (unsigned char *)malloc(dcap = 1024);
        if (!digits) abort();
        digits[ndig++] = (unsigned char)s;
    }
    while (ndig <= (size_t)(n - OFF)) next_digit();
    return (seq_t)digits[n - OFF];
}
