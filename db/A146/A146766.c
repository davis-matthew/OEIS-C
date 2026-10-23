/*
 * A146766  Triangle read by rows: expansion of p(x,n)=If[n == 0, 1, (x + 1)^n
 *          + 2^(n - 2)*Sum[Binomial[n, m]*x^m*(1 + x^(n - 2*m)), {m, 1, n -
 *          1}]].
 * https://oeis.org/A146766
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, less
 * Formula:   For row r (0‑based) with values a_{r,k} (k=0..r),
 *            a_{r,0}=a_{r,r}=1 and for 1≤k≤r-1
 *            a_{r,k}=binom(r,k)*(1+2^{r-1}). The sequence a(n) is the
 *            triangle read by rows
 * Range:     u8 n <= 22, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

static seq_t binomial_simple(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res *= (n - k + i);
        res /= i;
    }
    return res;
}

/* Computes the nth term of OEIS A146766 */
seq_t a(idx_t n)
{
    /* determine row r and column k */
    idx_t r = (idx_t)((sqrt(1.0 + 8.0 * (double)n) - 1.0) / 2.0);
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    while (r * (r + 1) / 2 > n) --r;

    idx_t row_start = r * (r + 1) / 2;
    idx_t k = n - row_start;

    /* edge values */
    if (k == 0 || k == r) {
        return (seq_t)1;
    }

    /* interior value */
    seq_t bin = binomial_simple(r, k);

    /* compute 2^(r-1) safely using repeated shift */
    seq_t two = 1;
    for (idx_t i = 0; i < r - 1; ++i) {
        two <<= 1;
    }
    seq_t factor = two + 1;

    return bin * factor;
}
