/*
 * A097229  Triangle read by rows: number of Motzkin paths by length and by
 *          number of humps.
 * https://oeis.org/A097229
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) equals T(r,k), where r is the row of the flattened *
 *            triangle that contains index n and k is the number of humps in
 *            that * entry. T(r,0) = 1. For k > 0, * T(r,k) = Σ_{i=k}^{⌊r/2⌋}
 *            binomial(r,2*i) * narayana(i,k), * where narayana(i,k) is the
 *            Narayana number. * * The triangle is flattened by...
 * Range:     u8 n <= 19, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial, narayana, row_and_pos
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/narayana.h"
#include "../utils/row_and_pos.h"

/* Compute T(r,k) via the defining summation. */
static seq_t T_val(idx_t r, idx_t k)
{
    if (k == 0)            /* T(r,0) = 1 for every r */
        return (seq_t)1;

    idx_t max_i = r / 2;   /* largest i such that 2*i <= r */
    seq_t sum = 0;

    /* Only i ≥ k contribute (else narayana(i,k)=0). */
    for (idx_t i = k; i <= max_i; ++i) {
        seq_t comb = binomial(r, 2 * i);
        seq_t nar  = narayana(i, k);
        sum += comb * nar;
    }
    return sum;
}

/* Return the n-th term of the flattened triangle. */
seq_t a(idx_t n)
{
    idx_t pos;
    idx_t row = row_and_pos(n, &pos);
    idx_t len = (row <= 1) ? 1 : (row / 2) + 1;

    idx_t k;
    if (pos == len - 1)   /* last position corresponds to k = 0 */
        k = 0;
    else                  /* positions 0 .. len-2 correspond to k = 1 .. floor(r/2) */
        k = pos + 1;

    return T_val(row, k);
}
