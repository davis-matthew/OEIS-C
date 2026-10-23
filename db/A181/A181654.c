/*
 * A181654  An eigentriangle of the number triangle A181653.
 * https://oeis.org/A181654
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

/*  Sequence a(n) is the n-th entry of the eigentriangle of
 *  the number triangle A181653, enumerated row‑wise.
 *  The triangle is defined column‑wise by the generating
 *  functions
 *
 *      g_k(x) = x^k * (1 + x - x^2) / (1 - 3x^2)          if k even
 *      g_k(x) = x^k * (1 + x - x^2 + x^3) / (1 - 3x^2)  if k odd
 *
 *  The coefficient of x^n in g_k(x) gives the triangle entry
 *  T(r,c) with r = n and c = k.  The a(n) sequence is simply
 *  the values of T(r,c) where r is the row index and
 *  c = n – r(r+1)/2 is the column index within that row.
 *
 *  The coefficient can be written as
 *
 *      T(r,c) = Σ_{m≥0, 2m≤r-c} coeff_{r-c-2m} * 3^m
 *
 *  where the 'coeff' array equals [1,1,-1] for even c and
 *  [1,1,-1,1] for odd c.  Only the indices 0,1,2 (odd: 0,1,2,3)
 *  contribute.  This implementation sums the positive and
 *  negative contributions separately to avoid unsigned underflow.
 */

seq_t a(idx_t n)
{
    /* Determine row r: r*(r+1)/2 <= n < (r+1)*(r+2)/2      */
    idx_t r = (idx_t)((sqrtl(8.0L * n + 1.0L) - 1.0L) / 2.0L);
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    while (r * (r + 1) / 2 > n) --r;

    /* Column index within the row                          */
    idx_t k = n - r * (r + 1) / 2;   /* k ∈ [0, r]              */
    idx_t d = r - k;                 /* distance from column start */

    seq_t sum_pos = 0;   /* terms that add */
    seq_t sum_neg = 0;   /* terms that subtract */
    seq_t pow3   = 1;    /* 3^m */

    for (idx_t m = 0; 2 * m <= d; ++m)
    {
        idx_t idx = d - 2 * m;     /* index inside the column */

        if (idx == 0 || idx == 1)
        {
            sum_pos += pow3;
        }
        else if (idx == 2)
        {
            sum_neg += pow3;
        }
        else if ((k & 1U) && idx == 3)
        {
            sum_pos += pow3;
        }

        pow3 *= (seq_t)3;
    }

    return sum_pos - sum_neg;
}
