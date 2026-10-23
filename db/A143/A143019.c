/*
 * A143019  Infinite square array read by antidiagonals: a(q,n) is the
 *          coefficient of z^n in the series expansion of C(z)^q/(1-4z)^(3/2),
 *          where C(z) = (1-sqrt(1-4z))/(2z) is the Catalan function (q,n =
 *          0,1,2,...).
 * https://oeis.org/A143019
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) computed by reading the infinite array a(q,n) along
 *            antidiagonals: a(q,n) = Sum_{i=0..n} 4^i * binomial(2n-2i+q,
 *            n-i)
 * Range:     u8 n <= 13, u16 n <= 42, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/binomial.h"

/* Compute a(q,n) via the explicit sum.
   Parameters are non‑negative integer indices. */
static seq_t compute_a_qn(idx_t q, idx_t n_para)
{
    seq_t sum  = 0;
    seq_t pow4 = 1;          /* 4^0 */
    for (idx_t i = 0; i <= n_para; ++i)
    {
        idx_t n_val = 2 * n_para - 2 * i + q;
        idx_t k_val = n_para - i;
        seq_t term = pow4 * binomial(n_val, k_val);
        sum += term;
        pow4 *= 4;
    }
    return sum;
}

/* Main sequence function a(n). */
seq_t a(idx_t N)
{
    /* Find antidiagonal index d such that
       d(d+1)/2 <= N < (d+1)(d+2)/2. */
    idx_t d = 0;
    {
        double nd = (double)N;
        d = (idx_t)floor((sqrt(8.0 * nd + 1.0) - 1.0) * 0.5);
        while ((d + 1) * (d + 2) / 2 <= N) ++d;
        while (d * (d + 1) / 2 > N) --d;
    }

    idx_t tprev  = d * (d + 1) / 2;   /* entries before diagonal d */
    idx_t offset = N - tprev;         /* position within diagonal d   */
    idx_t q      = d - offset;        /* row index  (q)              */
    idx_t n_para = offset;            /* column index (n)            */

    return compute_a_qn(q, n_para);
}
