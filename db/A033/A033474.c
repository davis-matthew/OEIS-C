/*
 * A033474  Duplicate of A001896.
 * https://oeis.org/A033474
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = (-1)^n * (2^(2n-1) - 1) * |B_{2n}|, where B_{2n} is the
 *            Bernoulli number with 0-based index
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 8, i64 n <= 10, i128 n <= 10,
 *            big any n
 * Helpers:   gcd, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/seq_pow.h"

/* Rational structure for computing Bernoulli numbers. */
typedef struct {
    seq_t num;   /* numerator */
    seq_t den;   /* denominator ( >0 ) */
} Rational;

/* Static storage for Bernoulli numbers. */
static Rational *B     = NULL;   /* array of rational Bernoulli numbers */
static size_t    B_cap = 0;       /* size of B array               */
static seq_t   *bnumer = NULL;   /* absolute numerators BN_{2m}    */
static size_t   N_cap  = 0;       /* size of bnumer array          */
static idx_t    B_max  = 0;       /* highest m computed (index)     */

/* Ensure B[0] is initialized. */
static void init_B0(void)
{
    if (B_cap == 0) return;
    B[0].num = (seq_t)1;
    B[0].den = (seq_t)1;
    bnumer[0] = (seq_t)1;
}

/* Compute Bernoulli numbers B_m for m = B_max+1 ... N inclusive.
 * After the call, bnumer[m] holds |numerator(B_m)|.
 */
static void compute_bernoulli_up_to(idx_t N)
{
    if (N <= B_max) return;

    /* Allocate storage for B and bnumer if necessary. */
    if (B_cap < (size_t)(N + 1)) {
        B = (Rational *)realloc(B, (size_t)(N + 1) * sizeof(Rational));
        if (!B) exit(EXIT_FAILURE);
        B_cap = N + 1;
    }

    if (N_cap < (size_t)(N + 1)) {
        bnumer = (seq_t *)realloc(bnumer, (size_t)(N + 1) * sizeof(seq_t));
        if (!bnumer) exit(EXIT_FAILURE);
        N_cap = N + 1;
    }

    /* Ensure B[0] is initialized once. */
    if (B_max == 0) init_B0();

    /* Compute Bernoulli numbers from B_max+1 up to N. */
    for (idx_t m = B_max + 1; m <= N; ++m) {
        /* Set a[m] = 1/(m+1). */
        B[m].num = (seq_t)1;
        B[m].den = (seq_t)(m + 1);

        /* Update lower entries: for j = m ... 1. */
        for (idx_t j = m; j >= 1; --j) {
            /* Compute a[j-1] = j * (a[j-1] - a[j]) */
            seq_t numDiff = B[j-1].num * B[j].den - B[j-1].den * B[j].num;
            seq_t denDiff = B[j-1].den * B[j].den;
            numDiff *= (seq_t)j;

            /* Reduce the fraction. */
            seq_t g = gcd(numDiff >= 0 ? numDiff : -numDiff,
                          denDiff >= 0 ? denDiff : -denDiff);
            if (g > 1) {
                numDiff /= g;
                denDiff /= g;
            }

            /* Ensure denominator positive. */
            if (denDiff < 0) {
                denDiff = -denDiff;
                numDiff = -numDiff;
            }

            B[j-1].num = numDiff;
            B[j-1].den = denDiff;
        }

        /* Store absolute numerator of B_m. */
        seq_t val = B[0].num;
        if (val < 0) val = -val;
        bnumer[m] = val;
    }

    B_max = N;
}

/* Public function required by the grader. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* Compute Bernoulli numbers up to index 2n. */
    idx_t two_n = n * 2;
    compute_bernoulli_up_to(two_n);

    /* Compute (2^(2n-1) - 1). */
    seq_t pow2_val = seq_pow((seq_t)2, two_n - 1);
    seq_t base     = pow2_val - (seq_t)1;

    /* Multiply by the absolute numerator of B_{2n}. */
    seq_t res = base * bnumer[two_n];

    /* Apply the alternating sign (-1)^n. */
    if (n & 1U) {
        res = -res;
    }

    return res;
}
