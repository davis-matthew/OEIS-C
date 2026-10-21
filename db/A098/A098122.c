/*
 * A098122  Let (A,B)=(a(2*n),a(2*n+1)), then (A,B) is (even,odd), gcd(A,B)=1
 *          and A^2 + B^2 = 5^n. Note: a(0)=0.
 * https://oeis.org/A098122
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 28, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A098122 - a(n) = 2-dimensional Lucas sequence from powers of (2+i)
 *
 *  The n-th term a(n) is obtained from the unique coprime pair (even,odd)
 *  solving A²+B² = 5^(n/2) with n even, the pair being the coordinates
 *  of the Gaussian integer (2+i)^(n/2).  The sequence interleaves the
 *  even and odd legs of the right triangle with hypotenuse 5^k.
 *
 *  Implementation details:
 *  - The power (2+i)^k is computed by fast exponentiation with complex
 *    multiplication.  The real and imaginary parts are stored as signed
 *    2's‑complement values encoded by an unsigned absolute value plus
 *    a sign flag.
 *  - All arithmetic is performed with the sequence type seq_t, which
 *    supports +, -, *, comparisons and bitwise ops, as required by the
 *    specification.
 *  - The function a(idx_t n) can be called in arbitrary order; no
 *    memoisation is required because each call runs in O(log n) time.
 */

#include <stdbool.h>

/* ----- Signed integer helper (abs + sign bit) ----- */
typedef struct {
    seq_t abs;      /* absolute value (unsigned) */
    bool neg;       /* true if negative */
} signed_int;

/* multiply two signed integers */
static inline signed_int signed_mul(signed_int a, signed_int b) {
    signed_int r;
    r.abs = a.abs * b.abs;
    r.neg = a.neg ^ b.neg;
    return r;
}

/* add two signed integers */
static inline signed_int signed_add(signed_int a, signed_int b) {
    signed_int r;
    if (a.neg == b.neg) {            /* same sign: add magnitudes */
        r.abs = a.abs + b.abs;
        r.neg = a.neg;
    } else {                          /* different signs: subtract magnitudes */
        if (a.abs >= b.abs) {
            r.abs = a.abs - b.abs;
            r.neg = a.neg;
        } else {
            r.abs = b.abs - a.abs;
            r.neg = b.neg;
        }
    }
    return r;
}

/* negate a signed integer */
static inline signed_int signed_neg(signed_int a) {
    a.neg = !a.neg;
    return a;
}

/* subtract b from a (a - b) */
static inline signed_int signed_sub(signed_int a, signed_int b) {
    return signed_add(a, signed_neg(b));
}

/* ----- Complex integer helper (real+imag) ----- */
typedef struct {
    signed_int re;   /* real part: abs + sign */
    signed_int im;   /* imaginary part */
} complex_t;

/* multiply two complex integers */
static inline complex_t complex_mul(complex_t a, complex_t b) {
    /* real part: a.re * b.re - a.im * b.im */
    signed_int t1 = signed_mul(a.re, b.re);
    signed_int t2 = signed_mul(a.im, b.im);
    signed_int cr = signed_sub(t1, t2);

    /* imaginary part: a.re * b.im + a.im * b.re */
    signed_int t3 = signed_mul(a.re, b.im);
    signed_int t4 = signed_mul(a.im, b.re);
    signed_int ci = signed_add(t3, t4);

    complex_t r;
    r.re = cr;
    r.im = ci;
    return r;
}

/* fast exponentiation: (2+i)^k */
static inline complex_t pow_2plus_i(idx_t k) {
    complex_t result;
    result.re.abs = 1; result.re.neg = false;
    result.im.abs = 0; result.im.neg = false;

    complex_t base;
    base.re.abs = 2; base.re.neg = false;
    base.im.abs = 1; base.im.neg = false;

    while (k) {
        if (k & 1u) result = complex_mul(result, base);
        base = complex_mul(base, base);
        k >>= 1;
    }
    return result;
}

/* ----- Sequence function ----- */
seq_t a(idx_t n) {
    idx_t p = n >> 1;                 /* power index */
    complex_t c = pow_2plus_i(p);

    seq_t re = c.re.abs;
    seq_t im = c.im.abs;

    /* Determine which absolute value is the even leg */
    if (n & 1u) {                     /* odd index → odd leg */
        return (p & 1u) ? im : re;
    } else {                          /* even index → even leg */
        return (p & 1u) ? re : im;
    }
}
