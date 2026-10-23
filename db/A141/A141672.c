/*
 * A141672  Triangle defined by T(n, k) = (n/k)*MoebiusMu(n) if n mod k is
 *          zero and T(n, 0) = n+1, read by rows.
 * https://oeis.org/A141672
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = T(r, k) where r = floor((sqrt(8n+1)-1)/2), k = n -
 *            r*(r+1)/2, T(r,0) = r+1, T(r,k) = (r/k)*mu(r) if r%k==0 else 0
 *            (mu is Möbius)
 * Range:     i8 n <= 5150, i16 n <= 5150, i32 n <= 5150, i64 n <= 5150,
 *            i128 n <= 5150, big any n
 * Helpers:   mobius_val
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mobius_val.h"

/* locate the row r (0‑based) containing the overall index n
   and the start index of that row (the cumulative count up to row r-1). */
static inline void locate_row(idx_t n, idx_t *r, idx_t *row_start)
{
    *r = 0;
    *row_start = 0;

    while ((*row_start + *r + 1) <= n) {
        *row_start += *r + 1;   /* skip this row */
        ++(*r);
    }
}

seq_t a(idx_t n)
{
    idx_t r;
    idx_t row_start;
    locate_row(n, &r, &row_start);

    idx_t k = n - row_start;

    if (k == 0)                      /* first element of the row */
        return (seq_t)(r + 1);

    if (r % k != 0)                  /* not divisible → 0 */
        return (seq_t)0;

    seq_t mu = mobius_val(r);
    seq_t div = (seq_t)(r / k);      /* integer division */
    return mu * div;
}
