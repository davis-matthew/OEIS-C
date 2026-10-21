/*
 * A067764  Numerators of the coefficients in exp(x/(1-x)) power series.
 * https://oeis.org/A067764
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   gcd, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A067764 : Numerators of the coefficients in exp(x/(1-x))
    The coefficients b_n of exp(x/(1-x)) satisfy
        b_0 = 1
        b_n = (1/n) * sum_{k=1}^{n} k * b_{n-k}
    We keep the fractions b_n = a_n / d_n in reduced form.
    While generating the series we maintain the sums
        S0 = sum_{j=0}^{n-1} b_j
        S1 = sum_{j=0}^{n-1} j * b_j
    Then
        b_n = (n * S0 - S1) / n
*/

/* required utilities */
#include <stdlib.h>
#include "../utils/gcd.h"          /* seq_t gcd(seq_t a, seq_t b); */
#include "../utils/mul_small.h"   /* seq_t mul_small(seq_t x, unsigned int m); */

/* helper: reduce fraction to lowest terms */
static void reduce(seq_t *num, seq_t *den)
{
    seq_t g = gcd(*num, *den);
    if (g != 0) {
        *num /= g;
        *den /= g;
    }
}

/* helper: add two fractions a1/d1 + a2/d2 = res_num/res_den */
static void add_frac(seq_t a1, seq_t d1, seq_t a2, seq_t d2,
                     seq_t *res_num, seq_t *res_den)
{
    seq_t g = gcd(d1, d2);
    seq_t mult1 = d2 / g;
    seq_t mult2 = d1 / g;
    seq_t den = (d1 / g) * d2;
    seq_t num = a1 * mult1 + a2 * mult2;
    reduce(&num, &den);
    *res_num = num;
    *res_den = den;
}

/* helper: subtract two fractions a1/d1 - a2/d2 = res_num/res_den */
/* assumes the result is non‑negative */
static void sub_frac(seq_t a1, seq_t d1, seq_t a2, seq_t d2,
                     seq_t *res_num, seq_t *res_den)
{
    seq_t g = gcd(d1, d2);
    seq_t mult1 = d2 / g;
    seq_t mult2 = d1 / g;
    seq_t den = (d1 / g) * d2;
    seq_t num1 = a1 * mult1;
    seq_t num2 = a2 * mult2;
    seq_t num = num1 - num2;   /* num1 >= num2 by construction */
    if (num == 0) {
        *res_num = 0;
        *res_den = 1;
        return;
    }
    reduce(&num, &den);
    *res_num = num;
    *res_den = den;
}

/* cache for computed numerators a(n) */
static seq_t *cache     = NULL;      /* malloc'ed array of a(n) */
static size_t cached_cap = 0;        /* allocated capacity */
static size_t cached_len = 0;        /* number of values currently in cache */

/* running sums for the series:
   S0 = sum_{j=0}^{n-1} b_j      (num/den)
   S1 = sum_{j=0}^{n-1} j*b_j    (num/den) */
static seq_t S0_num = 0, S0_den = 1;
static seq_t S1_num = 0, S1_den = 1;

/* initialise static data on first call */
static void init_cache(void)
{
    if (cache != NULL) return;   /* already initialised */
    cache = (seq_t *)malloc(sizeof(seq_t));
    if (!cache) { cache = NULL; return; }
    cached_cap = 1;
    cached_len = 1;
    cache[0] = (seq_t)1;         /* a(0) = 1 */

    /* b_0 = 1/1 */
    S0_num = (seq_t)1;
    S0_den = (seq_t)1;

    /* S1 = 0 */
    S1_num = (seq_t)0;
    S1_den = (seq_t)1;
}

/* Compute a(n) = numerator of coefficient of x^n in exp(x/(1-x)) */
seq_t a(idx_t n)
{
    init_cache();
    if (!cache) return (seq_t)0;            /* allocation failed */

    /* extend cache up to requested index if necessary */
    if ((idx_t)cached_len <= n) {
        size_t new_cap = cached_cap;
        if (new_cap == 0) new_cap = 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failed */
        cache = tmp;
        cached_cap = new_cap;
    }

    /* compute new terms up to n */
    for (idx_t i = cached_len; i <= n; ++i) {
        /* compute t = i * S0 */
        seq_t t_num = mul_small(S0_num, (unsigned int)i);
        seq_t t_den = S0_den;

        /* compute S = t - S1 */
        seq_t S_num, S_den;
        sub_frac(t_num, t_den, S1_num, S1_den, &S_num, &S_den);

        /* b_i = S / i */
        seq_t bi_num = S_num;
        seq_t bi_den = mul_small(S_den, (unsigned int)i);
        reduce(&bi_num, &bi_den);

        /* store numerator a(i) */
        cache[i] = bi_num;

        /* update S0 = S0 + b_i */
        seq_t new_S0_num, new_S0_den;
        add_frac(S0_num, S0_den, bi_num, bi_den, &new_S0_num, &new_S0_den);
        S0_num = new_S0_num;
        S0_den = new_S0_den;

        /* update S1 = S1 + i * b_i */
        seq_t temp_num = mul_small(bi_num, (unsigned int)i);
        seq_t temp_den = bi_den;
        seq_t new_S1_num, new_S1_den;
        add_frac(S1_num, S1_den, temp_num, temp_den, &new_S1_num, &new_S1_den);
        S1_num = new_S1_num;
        S1_den = new_S1_den;
    }

    cached_len = (size_t)n + 1;
    return cache[n];
}
