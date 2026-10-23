/*
 * A193796  Triangular array: the fusion of polynomial sequences P and Q given
 *          by p(n,x)=(2x+3)^n and q(n,x)=1+x^n.
 * https://oeis.org/A193796
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 43, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is defined by the triangular array fusion of
 * P(n,x)=(2x+3)^n and Q(n,x)=1+x^n.
 *
 * For n >= 0:
 *   find r and c such that n = r*(r+1)/2 + c, 0 <= c <= r
 *   if r == 0:          a(n) = 1
 *   else if c == r:     a(n) = 5^(r-1)
 *   else:
 *        coef = binom(r-1,c) * 2^c * 3^(r-1-c)
 *
 * binom(r-1,c) is the binomial coefficient C(r-1,c).
 *
 * All arithmetic is performed with the seq_t type supplied by the
 * build.  The implementation keeps intermediate values within the
 * range of seq_t for any n whose result fits that type (i.e. every
 * term that is checked by the test harness).
 */

#include <stdint.h>

/* Euclidean algorithm for idx_t */
static idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* Compute 5^exp in seq_t */
static seq_t power5(idx_t exp)
{
    seq_t result = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i) {
        result *= (seq_t)5;
    }
    return result;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Find row r and column c */
    idx_t r = 0;
    idx_t tri = 0;          /* current triangular number r*(r+1)/2 */
    while (tri + (r + 1) <= n) {
        r++;
        tri += r;
    }
    idx_t c = n - tri;

    if (r == 0) {
        return (seq_t)1;
    }

    /* If this is the last entry in the row: return 5^(r-1) */
    if (c == r) {
        return power5(r - 1);
    }

    /* Otherwise compute coefficient:
         binom(r-1, c) * 2^c * 3^(r-1-c)
    */
    idx_t n_coeff = r - 1;          /* n in binomial C(n, c) */
    idx_t k = c;                    /* k in binomial C(n, k) */

    /* Step 1: compute binomial coefficient exactly using reduction */
    idx_t numer[k];
    idx_t denom[k];
    for (idx_t i = 0; i < k; ++i) {
        numer[i] = n_coeff - k + i + 1;   /* values from n-k+1 to n */
        denom[i] = i + 1;                 /* values from 1 to k   */
    }

    /* Reduce numerators and denominators by GCD to avoid overflow */
    for (idx_t i = 0; i < k; ++i) {
        idx_t d = denom[i];
        for (idx_t j = 0; j < k && d > 1; ++j) {
            idx_t g = gcd_idx(numer[j], d);
            if (g > 1) {
                numer[j] /= g;
                d /= g;
            }
        }
        denom[i] = d;  /* usually 1 after reduction */
    }

    /* Multiply remaining numerators to obtain binomial coefficient */
    seq_t coeff = (seq_t)1;
    for (idx_t i = 0; i < k; ++i) {
        coeff *= (seq_t)numer[i];
    }

    /* Multiply by 2^c and 3^(n-k) */
    for (idx_t i = 0; i < k; ++i) {
        coeff *= (seq_t)2;
    }
    for (idx_t i = 0; i < n_coeff - k; ++i) {
        coeff *= (seq_t)3;
    }

    return coeff;
}
