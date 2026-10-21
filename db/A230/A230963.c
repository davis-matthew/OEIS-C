/*
 * A230963  Values of y such that x^2 + y^2 = 73^n with x and y coprime and 0
 *          < x < y.
 * https://oeis.org/A230963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max(|Re((3+8i)^n)|, |Im((3+8i)^n)|)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is independent of the ``big’’ arithmetic library – only
 * the operators + - * and comparison (>, <, >=, <=, ==, !=) for
 * ``seq_t'' are required.
 *
 * A small signed‐wrapper is used to keep track of the possible
 * negative real and imaginary parts during the binary exponentiation.
 * This wrapper stores only a magnitude and a sign bit, so it works
 * for all allowed ``seq_t'' types (including arbitrary precision
 * classes) as long as the standard arithmetic and comparison
 * operators are defined for `seq_t`. */

#include <stdbool.h>          /* for bool */

typedef struct {
    seq_t mag;   /* magnitude of the signed value (absolute value) */
    bool sign;   /* false => positive, true => negative */
} signed_t;

/* Construct a signed_t value from an unsigned magnitude and a sign. */
static signed_t make_signed(seq_t m, bool s)
{
    signed_t x;
    x.mag = m;
    x.sign = s;
    return x;
}

/* Add two signed_t values. */
static signed_t add_signed(signed_t a, signed_t b)
{
    /* same sign: magnitudes add */
    if (a.sign == b.sign) {
        return make_signed(a.mag + b.mag, a.sign);
    }

    /* different signs: subtract smaller magnitude from larger */
    if (a.mag > b.mag) {
        return make_signed(a.mag - b.mag, a.sign);
    } else if (b.mag > a.mag) {
        return make_signed(b.mag - a.mag, b.sign);
    } else {                        /* equal magnitudes → zero */
        return make_signed((seq_t)0, false);
    }
}

/* Subtract b from a. */
static signed_t sub_signed(signed_t a, signed_t b)
{
    b.sign = !b.sign;               /* negate the second operand */
    return add_signed(a, b);
}

/* Multiply two signed_t values. */
static signed_t mul_signed(signed_t a, signed_t b)
{
    return make_signed(a.mag * b.mag, a.sign ^ b.sign);
}

/* A very small complex Gaussian integer type.  Each part is a signed_t. */
typedef struct {
    signed_t re;
    signed_t im;
} comp_t;

/* Multiply two complex numbers. */
static comp_t comp_mul(comp_t a, comp_t b)
{
    comp_t r;
    signed_t arb = mul_signed(a.re, b.re);
    signed_t aimb = mul_signed(a.im, b.im);
    r.re = sub_signed(arb, aimb);            /* real part */

    signed_t arim = mul_signed(a.re, b.im);
    signed_t aimr = mul_signed(a.im, b.re);
    r.im = add_signed(arim, aimr);           /* imaginary part */
    return r;
}

/* Compute (3+8i)^n using binary exponentiation. */
seq_t a(idx_t n)
{
    if (n == 0)               /* out of OEIS range – return 0 */
        return (seq_t)0;

    /* base = 3+8i */
    comp_t base;
    base.re = make_signed((seq_t)3, false);
    base.im = make_signed((seq_t)8, false);

    /* result = 1+0i */
    comp_t result;
    result.re = make_signed((seq_t)1, false);
    result.im = make_signed((seq_t)0, false);

    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1)
            result = comp_mul(result, base);
        base = comp_mul(base, base);
        exp >>= 1;
    }

    /* return max(|real|, |imag|) – their magnitudes are already stored */
    seq_t abs_re = result.re.mag;
    seq_t abs_im = result.im.mag;
    return (abs_re > abs_im) ? abs_re : abs_im;
}
