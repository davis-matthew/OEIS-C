/*
 * A047681  a(n) = 3^(2*n)*(3^(2*n)-1)*Bernoulli(2*n)/(2*n).
 * https://oeis.org/A047681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 5, i64 n <= 9, i128 n <= 14,
 *            big any n
 * Helpers:   gcd, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A047681
 * Formula: a(n) = 3^(2*n)*(3^(2*n)-1)*Bernoulli(2*n)/(2*n)
 * (offset 1)
 */

#include <assert.h>
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/pow3.h"

/* Rational numbers for Bernoulli numbers */
typedef struct {
    seq_t num;   /* numerator  (may be negative) */
    seq_t den;   /* denominator (always > 0) */
} Rat;

/* Reduce a rational to lowest terms, keeping the denominator positive. */
static void rat_reduce(Rat *r)
{
    if (r->den < 0) {
        r->den = -r->den;
        r->num = -r->num;
    }
    seq_t tmp_abs = r->num < 0 ? -r->num : r->num;
    seq_t g = gcd(tmp_abs, r->den);
    if (g > 1) {
        r->num /= g;
        r->den /= g;
    }
}

/* Cache for Bernoulli numbers B[0..] as rationals. */
static Rat *B_cache   = NULL;
static size_t B_cap   = 0;   /* allocated capacity */
static size_t B_max   = 0;   /* highest index computed */

/* Ensure the cache has at least 'need' slots. */
static void ensure_B_capacity(size_t need)
{
    if (need < B_cap)
        return;
    size_t newcap = need + 64;   /* some slack */
    Rat *tmp = (Rat *)realloc(B_cache, newcap * sizeof(Rat));
    if (!tmp)
        abort();
    B_cache = tmp;
    B_cap   = newcap;
}

/* Compute Bernoulli numbers B[0..need] (need >= 0). */
static void compute_B_upto(size_t need)
{
    if (need <= B_max)
        return;   /* already computed */

    if (!B_cache)
        ensure_B_capacity(need + 1);
    else if (need + 1 > B_cap)
        ensure_B_capacity(need + 1);

    /* B[0] = 1. */
    if (B_max == 0) {
        B_cache[0].num = 1;
        B_cache[0].den = 1;
    }

    for (size_t m = B_max + 1; m <= need; ++m) {
        /* SUM = Σ_{k=0}^{m-1} C(m+1,k) * B_k */
        Rat sum = {0, 1};
        seq_t coeff = 1;                     /* C(m+1,0) */
        seq_t m1 = (seq_t)(m + 1);

        for (size_t k = 0; k < m; ++k) {
            /* term = coeff * B_k */
            seq_t term_num = coeff * B_cache[k].num;
            seq_t term_den = B_cache[k].den;

            /* sum += term */
            seq_t new_num = sum.num * term_den + term_num * sum.den;
            seq_t new_den = sum.den * term_den;
            if (new_den < 0) {
                new_den = -new_den;
                new_num = -new_num;
            }
            seq_t g = gcd(new_num < 0 ? -new_num : new_num, new_den);
            if (g > 1) {
                new_num /= g;
                new_den /= g;
            }
            sum.num = new_num;
            sum.den = new_den;

            /* advance coeff to C(m+1, k+1) */
            if (k + 1 < m) {
                seq_t num = m1 - k;   /* (m+1) - k */
                seq_t den = k + 1;    /* k+1   */
                coeff = coeff * num;
                coeff = coeff / den;
            }
        }

        /* B_m = - SUM / (m+1) */
        seq_t denom_factor = (seq_t)(m + 1);
        Rat r = {
            -sum.num,
            sum.den * denom_factor
        };
        rat_reduce(&r);
        B_cache[m] = r;
    }
    B_max = need;
}

/* The required sequence function. */
seq_t a(idx_t n)
{
    assert(n >= 1);

    size_t need = (size_t)(2 * n);          /* need Bernoulli number B[2n] */
    compute_B_upto(need);

    seq_t A        = pow3(need);                     /* 3^(2n) */
    seq_t Bdiff    = A - 1;                          /* 3^(2n)-1 */
    seq_t numB     = B_cache[need].num;              /* numerator of B[2n] */
    seq_t denB     = B_cache[need].den;              /* denominator of B[2n] */
    seq_t denom    = (seq_t)need * denB;             /* (2n)*denB */

    seq_t result = 1;    /* will multiply factors after cancelling denom */

    /* Factor A */
    seq_t f = A;
    seq_t g = gcd(f, denom);
    if (g > 1) { f /= g; denom /= g; }
    result *= f;

    /* Factor Bdiff */
    f = Bdiff;
    g = gcd(f, denom);
    if (g > 1) { f /= g; denom /= g; }
    result *= f;

    /* Factor numB */
    f = numB;
    g = gcd(f, denom);
    if (g > 1) { f /= g; denom /= g; }
    result *= f;

    /* At this stage denom should be 1; nothing left to divide. */

    return result;
}
