/*
 * A048013  Number of nonempty subsets of {1,2,...,n} in which exactly 4/5 of
 *          the elements are <= (n-1)/3.
 * https://oeis.org/A048013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{t≥1, 4t ≤ ⌊(n-1)/3⌋, t ≤ n-⌊(n-1)/3⌋} C(⌊(n-1)/3⌋, 4t)
 *            · C(n-⌊(n-1)/3⌋, t)
 * Range:     u8 n <= 21, u16 n <= 36, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count binomial coefficient C(n,k) using GCD reduction to avoid
   overflow for values that fit in seq_t. */
#include "../utils/gcd.h"

/* Multiplicative binomial calculation with GCD simplification.
   Works for any build of seq_t, including 64‑bit and arbitrary‑precision.
   Result is truncated if it does not fit in seq_t, but that is acceptable
   for terms that exceed the type. */
static seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k == 0 || k == n) return 1;
    if (k > n - k) k = n - k;    /* use symmetry */

    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        seq_t num = n - k + i;   /* numerator factor */
        seq_t den = i;           /* denominator factor */

        /* Reduce by gcd between numerator and denominator */
        seq_t g = gcd(num, den);
        num /= g;
        den /= g;

        /* Reduce remaining denominator with current result */
        g = gcd(res, den);
        res /= g;
        den /= g;               /* den should now be 1 */

        /* multiply */
        res *= num;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* undefined for n=0, return 0 */

    /* l = ⌊(n-1)/3⌋, u = n - l */
    idx_t l = (n - 1) / 3;
    idx_t u = n - l;

    seq_t sum = 0;

    /* t must satisfy 4t ≤ l and t ≤ u */
    idx_t max_t = l / 4;
    if (max_t > u) max_t = u;

    for (idx_t t = 1; t <= max_t; ++t)
    {
        seq_t c1 = binom(l, 4 * t);
        seq_t c2 = binom(u, t);
        sum += c1 * c2;
    }
    return sum;
}
