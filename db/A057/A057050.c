/*
 * A057050  Let R(i,j) be the rectangle with antidiagonals 1; 2,3; 4,5,6; ...;
 *          each k is an R(i(k),j(k)) and A057050(n)=j(n^2).
 * https://oeis.org/A057050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = column index j of n² in the anti‑diagonal enumeration
 *            R(i,j) where the number k = n² lies on diagonal d (1‑based)
 *            defined by T_{d‑1} < k ≤ T_d with T_d = d(d+1)/2. Then j = d –
 *            pos + 1 , pos = k – T_{d‑1}
 * Range:     u8 n <= 5, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   floor_sqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt_seq.h"

seq_t a(idx_t n)
{
    seq_t k   = (seq_t)n * (seq_t)n;            /* square of the index */
    seq_t inside = (seq_t)8 * k + (seq_t)1;     /* 8k+1 for the root test */

    /* integer part of sqrt(8k+1) */
    seq_t s   = floor_sqrt_seq(inside);

    /* preliminary diagonal number */
    seq_t d0  = (s - (seq_t)1) / (seq_t)2;
    seq_t tri = d0 * (d0 + (seq_t)1) / (seq_t)2;

    seq_t d;
    if (tri < k) d = d0 + (seq_t)1;   /* need next diagonal */
    else         d = d0;              /* exact or lower diagonal */

    /* compute previous diagonal sum T_{d-1} */
    seq_t prev = (d - (seq_t)1) * d / (seq_t)2;

    /* position inside the diagonal (1‑based) */
    seq_t pos = k - prev;

    /* column index */
    seq_t j = d - pos + (seq_t)1;

    return j;
}
