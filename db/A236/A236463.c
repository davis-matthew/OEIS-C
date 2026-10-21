/*
 * A236463  Irregular triangle read by rows: T(n,k) = Sum_{i=0..k} (-1)^i *
 *          binomial(4*n+1,i) * binomial(k+4-i,4)^n, 0 <= k <= 4*(n-1).
 * https://oeis.org/A236463
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) concatenates the irregular triangle T(n,k): * T(n,k) =
 *            Σ_{i=0..k} (-1)^i * binomial(4*n+1,i) * binomial(k+4-i,4)^n *
 *            for 0 ≤ k ≤ 4*(n-1). a(1) = T(1,0), a(2) = T(1,1), a(3) =
 *            T(2,0), …
 * Range:     u8 n <= 5, u16 n <= 18, u32 n <= 33, u64 n <= 98, u128 n <= 210,
 *            big any n
 * Helpers:   binomial, pow_seq, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/binomial.h"
#include "../utils/pow_seq.h"
#include "../utils/seq_mul.h"

/* Number of terms up to and including row r (rows are 1‑based).  */
static inline idx_t terms_upto(idx_t r)
{
    return r * (2 * r - 1);
}

/* Row number (1‑based) of the term whose overall index is pos (1‑based). */
static inline idx_t row_of(idx_t pos)
{
    if (pos == 0) return 0;
    double d = sqrt((double)(8.0L * pos + 1.0L));
    idx_t r  = (idx_t)((1.0L + d) / 4.0L);
    while (r * (2 * r - 1) < pos) ++r;
    while (r > 0 && (r - 1) * (2 * (r - 1) - 1) >= pos) --r;
    return r;
}

/* Value of the irregular triangle entry T(n,k). */
static seq_t T_row_k(idx_t n, idx_t k)
{
    seq_t pos_sum = 0;
    seq_t neg_sum = 0;

    for (idx_t i = 0; i <= k; ++i) {
        seq_t bin1 = binomial(4 * n + 1, i);           /* C(4n+1,i) */
        seq_t base = binomial(k + 4 - i, 4);           /* C(k+4-i,4) */
        seq_t powv = pow_seq(base, (int)n);            /* base^n  */
        seq_t term = seq_mul(bin1, powv);              /* bin1 * powv */

        if ((i & 1) == 0)
            pos_sum += term;
        else
            neg_sum += term;
    }
    return pos_sum - neg_sum;   /* guaranteed non‑negative */
}

/* Public entry point. */
seq_t a(idx_t idx)
{
    if (idx < 1) return 0;              /* defined only for n≥1 */
    idx_t r   = row_of(idx);                                /* row number (1‑based) */
    idx_t prev= terms_upto(r - 1);                          /* terms before this row */
    idx_t k   = idx - prev - 1;                             /* column index (0‑based) */
    return T_row_k(r, k);
}
