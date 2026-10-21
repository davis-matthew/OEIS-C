/*
 * A076658  Numerator of f(n), where for n > 2, f(n) = (n-1)/f(n-1) +
 *          (n-2)/f(n-2), with f(1)=1, f(2)=2.
 * https://oeis.org/A076658
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of the rational number f(n) defined by f(1) =
 *            1, f(2) = 2, f(n) = (n-1)/f(n-1) + (n-2)/f(n-2) (n > 2) The
 *            function returns a(n) for any n ≥ 1. It memoises computed values
 *            so calls may appear in any order. The implementation uses only
 *            integer arithmetic on seq_t and idx_t, reducin...
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 8, u64 n <= 10, u128 n <= 11,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>                 /* malloc, realloc */
#include "../utils/gcd.h"          /* seq_t gcd(seq_t a, seq_t b) */

static seq_t *num = NULL;         /* numerator of f(n) */
static seq_t *den = NULL;         /* denominator of f(n) */
static size_t cap = 0;            /* allocated capacity (0 means none) */
static idx_t  last = 0;           /* largest n that has been computed */

/* Ensure the cache can hold indices up to n (inclusive). */
static void ensure_capacity(idx_t n)
{
    if (cap >= (size_t)n + 1)
        return;                     /* already large enough */

    size_t newcap = cap ? cap * 2 : 8;   /* start with 8, double each time */
    while (newcap <= (size_t)n + 1)
        newcap <<= 1;                     /* grow until big enough */

    num = (seq_t *)realloc(num, (newcap + 1) * sizeof(seq_t));
    den = (seq_t *)realloc(den, (newcap + 1) * sizeof(seq_t));
    cap = newcap;
}

/* Initialise base cases if this is the first call. */
static void init_base(void)
{
    if (last == 0) {
        ensure_capacity(2);
        num[1] = (seq_t)1;   den[1] = (seq_t)1;
        num[2] = (seq_t)2;   den[2] = (seq_t)1;
        last = 2;
    }
}

seq_t a(idx_t n)
{
    init_base();

    /* Return cached value if already computed. */
    if (n <= last)
        return num[n];

    /* Grow cache to accommodate the new index. */
    ensure_capacity(n);

    /* Compute successive values up to n. */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* Previous terms */
        seq_t n1 = num[i - 1];   /* numerator of f(i-1) */
        seq_t d1 = den[i - 1];   /* denominator of f(i-1) */
        seq_t n2 = num[i - 2];   /* numerator of f(i-2) */
        seq_t d2 = den[i - 2];   /* denominator of f(i-2) */

        /* t1 = (i-1)/f(i-1) = (i-1)*d1 / n1 */
        seq_t t1_num = (seq_t)(i - 1) * d1;
        seq_t t1_den = n1;

        /* t2 = (i-2)/f(i-2) = (i-2)*d2 / n2 */
        seq_t t2_num = (seq_t)(i - 2) * d2;
        seq_t t2_den = n2;

        /* Sum the two fractions. */
        seq_t part1 = t1_num * t2_den;
        seq_t part2 = t2_num * t1_den;
        seq_t sum_num = part1 + part2;       /* numerator of f(i) before reduction */
        seq_t sum_den = t1_den * t2_den;     /* denominator of f(i) before reduction */

        /* Reduce fraction to lowest terms. */
        seq_t g = gcd(sum_num, sum_den);
        if (g > (seq_t)1) {
            sum_num /= g;
            sum_den /= g;
        }

        num[i] = sum_num;
        den[i] = sum_den;
    }

    last = n;
    return num[n];
}
