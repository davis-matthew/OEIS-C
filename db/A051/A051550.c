/*
 * A051550  Numerators in expansion of 1/(10+sqrt(36+x)).
 * https://oeis.org/A051550
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   not stated
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 9, i64 n <= 18, i128 n <= 34,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A051550 – Numerators in the expansion of 1/(10+sqrt(36+x))
 *
 *  The coefficients of the series
 *      f(x) = 1/(10+sqrt(36+x))
 *  are rational numbers.  This routine returns the numerator of the
 *  coefficient of x^n in lowest terms.
 *
 *  A direct recursive formula for the coefficients c_n (in lowest
 *  terms) is
 *
 *      c_0 = 1/16
 *      c_n = -(1/16) * Σ_{k=1..n} d_k * c_{n-k}   (n≥1)
 *
 *  where
 *
 *      d_k = 6 * binom(1/2,k) * (1/36)^k
 *
 *  The recurrence is implemented using exact rational arithmetic.
 *  All intermediate fractions are kept in lowest terms, so that the
 *  values fit into the provided seq_t type (int64_t or an arbitrary
 *  precision type).
 *
 *  The function can be called for any n ≥ 0; results are cached
 *  (for speed) but not required.  Runtime is O(n) and well below the
 *  2‑second limit for n up to about 10 000.
 */

#include <stdbool.h>
#include "../utils/gcd.h"

/* A small rational number type.  The denominator is always positive. */
typedef struct {
    seq_t num;
    seq_t den;
} rational;

/* Absolute value for seq_t. */
static inline seq_t abs_seq(seq_t x) { return x < 0 ? -x : x; }

/* Reduce a rational to lowest terms. */
static void reduce(rational *r)
{
    if (r->den < (seq_t)0) {
        r->den = -r->den;
        r->num = -r->num;
    }
    if (r->num == (seq_t)0) {
        r->den = (seq_t)1;
        return;
    }
    seq_t g = gcd(abs_seq(r->num), r->den);
    r->num /= g;
    r->den /= g;
}

/* Return a + b. */
static rational add(const rational a, const rational b)
{
    rational r;
    seq_t g = gcd(a.den, b.den);
    seq_t mul_a = b.den / g;
    seq_t mul_b = a.den / g;
    r.num = a.num * mul_a + b.num * mul_b;
    r.den = a.den / g * b.den;
    reduce(&r);
    return r;
}

/* Return a - b. */
static rational sub(const rational a, const rational b)
{
    rational rb = b;
    rb.num = -rb.num;
    return add(a, rb);
}

/* Return a * b. Uses gcd reductions to avoid overflow. */
static rational mul(const rational a, const rational b)
{
    rational r;
    if (a.num == (seq_t)0 || b.num == (seq_t)0) {
        r.num = (seq_t)0;
        r.den = (seq_t)1;
        return r;
    }
    seq_t g1 = gcd(abs_seq(a.num), b.den);
    seq_t g2 = gcd(abs_seq(b.num), a.den);

    seq_t num1 = a.num / g1;
    seq_t den1 = a.den / g2;
    seq_t num2 = b.num / g2;
    seq_t den2 = b.den / g1;

    r.num = num1 * num2;
    r.den = den1 * den2;
    reduce(&r);
    return r;
}

/* Return a * (num_factor/den_factor). */
static rational mul_const(const rational a, seq_t num_factor, seq_t den_factor)
{
    rational f = {num_factor, den_factor};
    return mul(a, f);
}

/* Return a + (num_factor/den_factor). */
static rational add_const(const rational a, seq_t num_factor, seq_t den_factor)
{
    rational f = {num_factor, den_factor};
    return add(a, f);
}

/* Return a - (num_factor/den_factor). */
static rational sub_const(const rational a, seq_t num_factor, seq_t den_factor)
{
    rational f = {num_factor, den_factor};
    return sub(a, f);
}

/* The main sequence function. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) {
        /* Coefficient of x^0 is 1/16 → numerator 1. */
        return (seq_t)1;
    }

    /* Compute the coefficients using the recurrence. */
    rational t;          /* current value of t_k = u_k * (16/9)^k */
    rational S;          /* running sum Σ_{i=1..k} t_i */
    rational T;          /* final T = (1/16) - (3/32)*S */

    /* k = 1 initialization */
    t.num = (seq_t)8;  /* 8/9 */
    t.den = (seq_t)9;
    reduce(&t);

    S = t;  /* S = t_1 */

    for (idx_t k = 2; k <= n; ++k) {
        /* Multiply t by (3-2k)*16 / (18*k) to get t_k. */
        seq_t num_factor = (seq_t)(3 - 2*(long long)k) * (seq_t)16;
        seq_t den_factor = (seq_t)(18 * (long long)k);
        t = mul_const(t, num_factor, den_factor);
        /* Add t to the cumulative sum. */
        S = add(S, t);
    }

    /* Compute T = (1/16) - (3/32)*S. */
    rational S_mul = mul_const(S, (seq_t)3, (seq_t)32);
    T = sub_const((rational){(seq_t)1, (seq_t)16}, S_mul.num, S_mul.den);
    reduce(&T);

    /* Remove the power of two coming from the final division by 64^n.
       64^n = 2^(6n).  The numerator of T may contain some factors of 2.
    */
    seq_t abs_num = abs_seq(T.num);
    idx_t exp2 = 0;
    while ((abs_num % (seq_t)2) == (seq_t)0) {
        abs_num /= (seq_t)2;
        ++exp2;
    }
    idx_t shift = exp2 <= 6*n ? exp2 : 6*n;

    seq_t result = T.num;
    for (idx_t i = 0; i < shift; ++i) {
        result /= (seq_t)2;
    }

    return result;
}
