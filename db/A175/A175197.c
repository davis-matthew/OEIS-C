/*
 * A175197  Array A(k,n) of the number of points of the A_k lattice with
 *          maximum infinity norm n, read by antidiagonals.
 * https://oeis.org/A175197
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = A(k, m) where k = t - (n - t(t+1)/2), m = n - t(t+1)/2, t
 *            = ⌊(√(8n+1)-1)/2⌋, and A(k, m) is the number of integer points
 *            of the A_k lattice with maximum infinity norm m. The enumeration
 *            follows antidiagonals of the table A(k,n). A(k, m) is computed
 *            via inclusion–exclusion
 * Range:     u8 n <= 15, u16 n <= 37, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   binom, seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/binom.h"
#include "../utils/seq_one.h"

static seq_t compute_F(idx_t k, idx_t n)
{
    /* Number of integer solutions to x1+...+x_{k+1}=0 with each |xi| ≤ n.
       This equals the number of solutions to y1+...+y_{k+1}=n(k+1) with
       0 ≤ yi ≤ 2n, computed by inclusion–exclusion. */
    if (k == 0)
        return seq_one();            /* always 1 */

    idx_t N     = n * (k + 1);             /* target sum for y_i */
    idx_t upper = 2 * n + 1;               /* 2n+1 = (2n)+1 in the formula */

    seq_t pos = 0;                         /* sum of terms with even j */
    seq_t neg = 0;                         /* sum of terms with odd j */

    for (idx_t j = 0; j <= k + 1; ++j)
    {
        idx_t rem = N - j * upper;         /* N - j*(2n+1) */
        if (rem > N)                       /* underflow for unsigned */
            break;
        if (rem == (idx_t)-1)              /* when N < j*upper */
            break;

        seq_t term1 = binom(k + 1, j);     /* C(k+1 , j) */
        seq_t term2 = binom(rem + k, k);   /* C(rem + k , k) */
        seq_t term  = term1 * term2;

        if (j & 1)
            neg += term;
        else
            pos += term;
    }

    return pos - neg;                      /* pos ≥ neg for all cases */
}

seq_t a(idx_t n)
{
    if (n == 0)               /* first term (k=0, m=0) */
        return seq_one();

    /* Compute t = floor((sqrt(8n+1)-1)/2) using double to keep it simple */
    double td = sqrt(8.0 * (double)n + 1.0);
    idx_t t = (idx_t)((td - 1.0) / 2.0);

    /* Adjust in case of rounding errors */
    while (((t + 1) * (t + 2)) / 2 <= n)
        ++t;
    while (t * (t + 1) / 2 > n)
        --t;

    /* Offset inside the t‑th antidiagonal */
    idx_t shift  = t * (t + 1) / 2;
    idx_t offset = n - shift;          /* 0 ≤ offset ≤ t */

    idx_t k = t - offset;              /* row index */
    idx_t m = offset;                  /* column index */

    if (m == 0)                        /* all A(k,0) equal 1 */
        return seq_one();

    /* A(k, m) = F(k, m) - F(k, m-1) */
    seq_t f_m    = compute_F(k, m);
    seq_t f_prev = compute_F(k, m - 1);
    return f_m - f_prev;
}
