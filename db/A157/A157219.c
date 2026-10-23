/*
 * A157219  Triangle T(n, k) = binomial(n*f(n,k), f(n,k)), where f(n, k) = k
 *          if k <= floor(n/2) otherwise n-k, read by rows.
 * https://oeis.org/A157219
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 58, u32 n <= 111, u64 n <= 311,
 *            u128 n <= 839, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Triangle A157219:  T(n,k) = binomial( n*f(n,k) , f(n,k) ),
    with  f(n,k) = k           if k ≤ ⌊n/2⌋
                n - k         otherwise
    Triangle coefficients are read row by row (offset 0).  This routine
    maps a linear index `n` to its row `r` and column `k`, computes `f`,
    and evaluates the binomial coefficient using a safe multiplicative
    algorithm that avoids overflow for all values that fit in the type
    `seq_t`.  */
#include "../utils/gcd.h"   /* provides seq_t gcd(seq_t, seq_t) */

/* Find the largest r with r*(r+1)/2 <= n  */
static inline idx_t
row_from_index(idx_t n)
{
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;
    return r;
}

/* Return column index within row r (0‑based) */
static inline idx_t
col_in_row(idx_t n, idx_t r)
{
    return n - r * (r + 1) / 2;
}

/* Compute f(r,k) according to the definition.  */
static inline idx_t
f_value(idx_t r, idx_t k)
{
    idx_t half = r / 2;           /* floor(r/2) */
    return (k <= half) ? k : r - k;
}

/* Safe binomial coefficient: C(n,k) when the result fits in seq_t.  */
/* Uses an incremental multiplicative algorithm with gcd reductions
   to keep intermediate values within bounds.  */
static seq_t
binomial_safe(idx_t n, idx_t k)
{
    if (k > n - k)
        k = n - k;            /* symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t num = (seq_t)(n - k + i);   /* numerator term   */
        seq_t den = (seq_t)i;             /* denominator term */
        seq_t g = gcd(num, den);          /* cancel numerator/denominator */
        num /= g;
        den /= g;
        g = gcd(res, den);                /* cancel result/denominator */
        res /= g;
        den /= g;
        /* At this point den == 1; multiply res by num.  */
        res = res * num;
        /* In theory den should be 1; just in case: */
        if (den != (seq_t)1)
            res /= den;
    }
    return res;
}

/* Public sequence routine */
seq_t
a(idx_t n)
{
    idx_t r = row_from_index(n);
    idx_t k = col_in_row(n, r);
    idx_t f = f_value(r, k);
    /* If f is 0, the result is always 1 (binomial(...,0)).  */
    if (f == 0)
        return 1;
    idx_t N = r * f;             /* n*f(n,k) in the definition.  */
    return binomial_safe(N, f);
}
