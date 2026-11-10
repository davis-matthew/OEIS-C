/*
 * A076349  A076340(A000290(n)), real part of squares mapped as defined in
 *          A076340, A076341.
 * https://oeis.org/A076349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A076340(n^2), A076340 = real part of the completely
 *            multiplicative f with f(2)=(2,0), f(p)=(p-1,1) if p==1 (mod 4),
 *            f(p)=(p+1,-1) if p==3 (mod 4); so a(n) = Re(prod over p^e || n
 *            of f(p)^(2e))
 * Range:     u8 n <= 14, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 49 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

typedef struct { seq_t m; int neg; } sg_t;
typedef struct { sg_t r, i; } cx_t;

static sg_t sg_add(sg_t a, sg_t b)
{
    sg_t c;
    if (a.neg == b.neg) {
        c.m = a.m + b.m;
        c.neg = a.neg;
    } else if (a.m >= b.m) {
        c.m = a.m - b.m;
        c.neg = a.neg;
    } else {
        c.m = b.m - a.m;
        c.neg = b.neg;
    }
    return c;
}

static sg_t sg_mul(sg_t a, sg_t b)
{
    sg_t c;
    c.m = a.m * b.m;
    c.neg = a.neg ^ b.neg;
    return c;
}

static sg_t sg_negate(sg_t a)
{
    a.neg = !a.neg;
    return a;
}

static cx_t cx_mul(cx_t a, cx_t b)
{
    cx_t c;
    c.r = sg_add(sg_mul(a.r, b.r), sg_negate(sg_mul(a.i, b.i)));
    c.i = sg_add(sg_mul(a.r, b.i), sg_mul(a.i, b.r));
    return c;
}

static cx_t cx_pow(cx_t base, idx_t e)
{
    cx_t res;
    res.r.m = 1; res.r.neg = 0;
    res.i.m = 0; res.i.neg = 0;
    while (e > 0) {
        if (e & 1) res = cx_mul(res, base);
        e >>= 1;
        if (e > 0) base = cx_mul(base, base);
    }
    return res;
}

static cx_t fprime(idx_t p)
{
    cx_t f;
    f.r.neg = 0;
    f.i.neg = 0;
    if (p == 2) {
        f.r.m = 2;
        f.i.m = 0;
    } else if ((p & 3) == 1) {
        f.r.m = (seq_t)p - 1;
        f.i.m = 1;
    } else {
        f.r.m = (seq_t)p + 1;
        f.i.m = 1;
        f.i.neg = 1;
    }
    return f;
}

seq_t a(idx_t n)
{
    cx_t res;
    res.r.m = 1; res.r.neg = 0;
    res.i.m = 0; res.i.neg = 0;
    idx_t m = n;
    for (idx_t p = 2; p <= m / p; ++p) {
        if (m % p != 0) continue;
        idx_t e = 0;
        while (m % p == 0) { m /= p; ++e; }
        res = cx_mul(res, cx_pow(fprime(p), 2 * e));
    }
    if (m > 1) {
        res = cx_mul(res, cx_pow(fprime(m), 2));
    }
    return res.r.m;
}
