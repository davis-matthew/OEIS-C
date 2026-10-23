/*
 * A158518  Numerator of Bernoulli(n, -3/7).
 * https://oeis.org/A158518
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   not stated
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 9, i64 n <= 19, i128 n <= 31,
 *            big any n
 * Helpers:   binom, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerator of Bernoulli(n, -3/7) – OEIS A158518
   a(n) = B_n(-3/7), the numerator of the nth Bernoulli polynomial
   evaluated at x = -3/7. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/binom.h"

/* Rational number stored as a quotient of seq_t integers,
   reduced to lowest terms with a positive denominator. */
typedef struct { seq_t num; seq_t den; } Rat;

/* Reduce a rational; denominator kept positive. */
static void reduce_rat(Rat *r) {
    if (r->num == 0) { r->den = 1; return; }
    seq_t g = gcd(r->num < 0 ? -r->num : r->num,
                  r->den < 0 ? -r->den : r->den);
    r->num /= g; r->den /= g;
    if (r->den < 0) { r->den = -r->den; r->num = -r->num; }
}

/* Construct a reduced rational. */
static Rat make_rat(seq_t num, seq_t den) { Rat r = {num, den}; reduce_rat(&r); return r; }

/* Multiply two rationals with maximal reduction before the
   intermediate product to keep values small. */
static Rat rat_mul(const Rat *a, const Rat *b) {
    seq_t g1 = gcd(a->num, b->den);
    seq_t g2 = gcd(b->num, a->den);
    seq_t num = (a->num / g1) * (b->num / g2);
    seq_t den = (a->den / g2) * (b->den / g1);
    return make_rat(num, den);
}

/* Multiply a rational by an integer with early reduction. */
static Rat rat_mul_int(const Rat *a, seq_t m) {
    seq_t g = gcd(a->den, m);
    seq_t num = a->num * (m / g);
    seq_t den = a->den / g;
    return make_rat(num, den);
}

/* Multiply a rational by a fixed fraction p/q with reduction. */
static Rat rat_mul_const(const Rat *a, seq_t p, seq_t q) {
    seq_t g1 = gcd(a->num, q);
    seq_t g2 = gcd(p, a->den);
    seq_t num = (a->num / g1) * (p / g2);
    seq_t den = (a->den / g2) * (q / g1);
    return make_rat(num, den);
}

/* Add two rationals with reduction before the final sum. */
static Rat rat_add(const Rat *a, const Rat *b) {
    seq_t g = gcd(a->den, b->den);
    seq_t lcm = (a->den / g) * b->den;           /* lcm of denominators */
    seq_t t1 = a->num * (b->den / g);
    seq_t t2 = b->num * (a->den / g);
    seq_t num = t1 + t2;
    return make_rat(num, lcm);
}

/* ------------------------------------------------------------ */
/* Caches for Bernoulli numbers B_k and powers of (-3/7).       */
/* ------------------------------------------------------------ */
static Rat *B_cache = NULL;   /* B_k, 0 <= k < B_size */
static size_t B_size = 0, B_cap = 0;

static Rat *Xpow_cache = NULL;   /* (-3/7)^k, 0 <= k < Xsize */
static size_t Xsize = 0, X_cap = 0;

/* Ensure B_cache holds Bernoulli numbers up to n. */
static void ensure_B_cache(size_t n) {
    if (n < B_size) return;

    size_t new_cap = n + 1;
    if (new_cap > B_cap) {
        size_t cap = new_cap * 2;
        Rat *tmp = (Rat *)realloc(B_cache, cap * sizeof(Rat));
        B_cache = tmp; B_cap = cap;
    }

    for (size_t k = B_size; k <= n; ++k) {
        if (k == 0) { B_cache[0] = make_rat((seq_t)1, (seq_t)1); }
        else {
            Rat sum = make_rat((seq_t)0, (seq_t)1);
            for (size_t i = 0; i < k; ++i) {
                seq_t coeff = binom((idx_t)(k + 1), (idx_t)i);
                Rat prod = rat_mul_int(&B_cache[i], coeff);
                sum = rat_add(&sum, &prod);
            }
            seq_t denom = (seq_t)(k + 1);
            B_cache[k] = make_rat(-sum.num, sum.den * denom);
        }
    }
    B_size = n + 1;
}

/* Ensure Xpow_cache holds powers of (-3/7) up to n. */
static void ensure_Xpow_cache(size_t n) {
    if (n < Xsize) return;

    size_t new_cap = n + 1;
    if (new_cap > X_cap) {
        size_t cap = new_cap * 2;
        Rat *tmp = (Rat *)realloc(Xpow_cache, cap * sizeof(Rat));
        Xpow_cache = tmp; X_cap = cap;
    }

    if (Xsize == 0) { Xpow_cache[0] = make_rat((seq_t)1, (seq_t)1); Xsize = 1; }
    for (size_t i = Xsize; i <= n; ++i) {
        Xpow_cache[i] = rat_mul_const(&Xpow_cache[i-1], (seq_t)-3, (seq_t)7);
    }
    Xsize = n + 1;
}

/* ------------------------------------------------------------ */
seq_t a(idx_t n) {
    size_t N = (size_t)n;
    ensure_B_cache(N);
    ensure_Xpow_cache(N);

    Rat sum = make_rat((seq_t)0, (seq_t)1);
    for (size_t k = 0; k <= N; ++k) {
        seq_t coeff = binom((idx_t)N, (idx_t)k);
        Rat part = rat_mul_int(&B_cache[k], coeff);
        Rat term = rat_mul(&part, &Xpow_cache[N-k]);
        sum = rat_add(&sum, &term);
    }
    return sum.num;
}
