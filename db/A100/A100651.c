/*
 * A100651  Numerator of 1 - Sum_{i=1..n} |Bernoulli(i)|.
 * https://oeis.org/A100651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 6, i16 n <= 14, i32 n <= 20, i64 n <= 30, i128 n <= 31,
 *            big any n
 * Helpers:   gcd_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A100651
    Numerator of 1 – Sum_{i=1}^{n-1} |B_i|   (B_i is the i‑th Bernoulli number)
    a(1)=1, a(2)=1, a(3)=1, a(4)=1, a(5)=3, a(6)=3, …  */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/gcd_seq.h"

/* Simple rational type, sign stored only on the numerator. */
typedef struct { seq_t num; seq_t den; } rational_t;

/* Normalise a rational: make denominator positive, cancel gcd, set zero to 0/1. */
static rational_t ratio_reduce(seq_t n, seq_t d)
{
    if (d < (seq_t)0) { n = -n; d = -d; }
    if (n == (seq_t)0) return (rational_t){(seq_t)0, (seq_t)1};
    seq_t g = gcd_seq(n >= (seq_t)0 ? n : -n, d);
    if (g > (seq_t)1) { n /= g; d /= g; }
    return (rational_t){n, d};
}

/* Addition of two rationals. */
static rational_t ratio_add(rational_t a, rational_t b)
{
    seq_t g = gcd_seq(a.den, b.den);
    seq_t l = (a.den / g) * b.den;
    seq_t n = a.num * (b.den / g) + b.num * (a.den / g);
    return ratio_reduce(n, l);
}

/* Subtraction of two rationals. */
static rational_t ratio_sub(rational_t a, rational_t b)
{
    seq_t g = gcd_seq(a.den, b.den);
    seq_t l = (a.den / g) * b.den;
    seq_t n = a.num * (b.den / g) - b.num * (a.den / g);
    return ratio_reduce(n, l);
}

/* Multiply a rational by an integer. */
static rational_t ratio_mul_int(rational_t a, seq_t k)
{
    return ratio_reduce(a.num * k, a.den);
}

/* Divide a rational by an integer (k>0). */
static rational_t ratio_div_int(rational_t a, seq_t k)
{
    return ratio_reduce(a.num, a.den * k);
}

/* Absolute value of a rational (sign on numerator only). */
static rational_t ratio_abs(rational_t a)
{
    seq_t n = a.num >= (seq_t)0 ? a.num : -a.num;
    return (rational_t){n, a.den};
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* For n=1 the sum is empty. */
    if (n <= (idx_t)1) return (seq_t)1;

    /* We need Bernoulli numbers B_0 … B_{n-1}. */
    size_t len = (size_t)n;                 /* number of entries needed */
    rational_t *B = (rational_t*)malloc(sizeof(rational_t) * len);
    if (!B) return (seq_t)0;                /* allocation failure */

    B[0] = (rational_t){(seq_t)1, (seq_t)1}; /* B_0 = 1 */
    rational_t sum = (rational_t){(seq_t)0, (seq_t)1}; /* Σ |B_i|, i=1..n-1 */

    /* n-1 may be 0 (already handled) so we start at m=1 up to n-1 inclusive. */
    for (size_t m = 1; m < len; ++m) {
        if (m == 1) {
            /* B_1 = -1/2 */
            B[m] = (rational_t){-(seq_t)1, (seq_t)2};
            sum = ratio_add(sum, ratio_abs(B[m]));
            continue;
        }
        if ((m & (size_t)1) != 0) {
            /* Odd Bernoulli numbers >1 are zero. */
            B[m] = (rational_t){(seq_t)0, (seq_t)1};
            continue;
        }

        /* Compute even Bernoulli number using Akiyama–Tanigawa recurrence.
           sum_k=0..m-1 binom(m+1,k) * B_k.   */
        rational_t acc = (rational_t){(seq_t)0, (seq_t)1};
        seq_t bin = 1; /* binom(m+1,0) */

        for (size_t k = 0; k < m; ++k) {
            rational_t term = ratio_mul_int(B[k], bin);
            acc = ratio_add(acc, term);

            /* Update bin to binom(m+1,k+1) */
            seq_t num = bin * (seq_t)(m + 1 - k);
            seq_t den = (seq_t)(k + 1);
            bin = num / den; /* exact integer division */
        }

        /* B_m = - acc / (m+1) */
        seq_t mm1 = (seq_t)(m + 1);
        rational_t neg = (rational_t){-acc.num, acc.den};
        B[m] = ratio_div_int(neg, mm1);

        /* Add its absolute value to the running sum. */
        sum = ratio_add(sum, ratio_abs(B[m]));
    }

    /* 1 - sum */
    rational_t one = (rational_t){(seq_t)1, (seq_t)1};
    rational_t result = ratio_sub(one, sum);

    free(B);
    return result.num; /* return numerator only */
}
