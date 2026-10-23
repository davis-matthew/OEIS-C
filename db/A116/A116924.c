/*
 * A116924  Triangle T(n,k) = B(k,n) - B(k-1,n) where B(n,m) = Sum_{i=0..n}
 *          binomial(m,i) (3*i+1).
 * https://oeis.org/A116924
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = C(r,k) * (3k+1), where r = floor((sqrt(8n+1)-1)/2), k = n
 *            - r(r+1)/2. This follows from T(n,k) = B(k,n)−B(k−1,n) =
 *            C(n,k)*(3k+1)
 * Range:     u8 n <= 30, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/gcd.h"

/* Compute the binomial coefficient C(n,k) for unsigned integer n,k
   (n,k are idx_t, stored into seq_t).  The multiplication is done
   with cancellation to avoid overflow when possible.  The result
   is returned in a seq_t value. */
static seq_t binom_uint64(idx_t n, idx_t k)
{
    seq_t bin = 1;
    /* we compute using the recurrence C(n,i+1)=C(n,i)*(n-i)/(i+1) */
    for (idx_t i = 1; i <= k; ++i)
    {
        seq_t numer = (seq_t)(n - i + 1);
        seq_t denom = (seq_t)i;

        /* cancel common factors between bin and denom */
        seq_t g = gcd(bin, denom);
        bin /= g;
        denom /= g;

        /* cancel remaining factors between numer and denom */
        g = gcd(numer, denom);
        numer /= g;
        denom /= g;  /* after cancellation denom should be 1 */

        /* multiply bin by numer (denom is 1) */
#if defined(__SIZEOF_INT128__)
        bin = (seq_t)(((__uint128_t)bin * numer));
#else
        bin = bin * numer;
#endif
    }
    return bin;
}

/* Find the row index r and column index k corresponding to the
   linear index n in the triangular enumeration of T(r,k).  The
   row r satisfies r(r+1)/2 <= n < (r+1)(r+2)/2.  The column is
   k = n - r(r+1)/2. */
static void row_col_from_index(idx_t n, idx_t *r_out, idx_t *k_out)
{
    /* Initial estimate using the quadratic formula with double
       arithmetic.  No 'math.h' dependency beyond sqrt(). */
    double d = sqrt((double)(8.0 * (double)n + 1.0));
    idx_t r = (idx_t)((d - 1.0) / 2.0);

    /* Correct possible rounding errors by simple adjustment. */
    idx_t cum_before = r * (r + 1) / 2;
    while (cum_before > n)
    {
        --r;
        cum_before = r * (r + 1) / 2;
    }
    while ((r + 1) * (r + 2) / 2 <= n)
    {
        ++r;
    }

    idx_t k = n - r * (r + 1) / 2;
    *r_out = r;
    *k_out = k;
}

seq_t a(idx_t n)
{
    idx_t r, k;
    row_col_from_index(n, &r, &k);

    /* T(r,k) = C(r,k) * (3k+1) */
    seq_t comb = binom_uint64(r, k);
    seq_t mult = (seq_t)(3 * k + 1);
#if defined(__SIZEOF_INT128__)
    return (seq_t)(((__uint128_t)comb * mult));
#else
    return comb * mult;
#endif
}
