/*
 * A188646  Array of a(n)=a(n-1)*k-((k-1)/(k^n)) where a(0)=1 and
 *          k=(sqrt(x^2-1)+x)^2 for integers x>=1.
 * https://oeis.org/A188646
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   A(n,k) = Sum_{j=0..k}
 *            binomial(2*k+1,2*j+1)*(n^2-1)^(k-j)*n^(2*j)
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"

/*  Computes the A188646 square array flattened by antidiagonals.
 *  The mapping converts the linear index n into coordinates (row, col)
 *  where row starts at 1 and col starts at 0.  The value is then
 *  calculated using the closed‑form sum above.
 */
seq_t a(idx_t n)
{
    /* Find diagonal index s such that  s*(s+1)/2 > n . */
    idx_t s = 1;
    while ( (__int128)s * (s + 1) / 2 <= n )
        ++s;

    /* Number of entries before this diagonal. */
    idx_t prev   = (idx_t)(( (__int128)(s - 1) * s ) / 2);
    idx_t offset = n - prev;

    /* Convert to array coordinates: row = offset+1, col = (s-1)-offset. */
    idx_t row = offset + 1;               /* 1‑based row index */
    idx_t col = (s - 1) - offset;         /* 0‑based column index */

    seq_t row_s = (seq_t)row;
    seq_t d = row_s * row_s;             /* d = row^2   */
    seq_t c = d - (seq_t)1;              /* c = row^2-1 */

    /* Special case when (row^2-1) == 0 : only the term with j = col contributes. */
    if (c == (seq_t)0) {
        /* For row = 1 we have d = 1 and the sum reduces to d^col. */
        seq_t pow_d = (seq_t)1;
        for (idx_t i = 0; i < col; ++i)
            pow_d *= d;
        return pow_d;
    }

    /* Pre‑compute c^col (the highest power required). */
    seq_t pow_c = (seq_t)1;
    for (idx_t i = 0; i < col; ++i)
        pow_c *= c;

    seq_t pow_d = (seq_t)1;            /* current d^j (starts with j = 0) */
    seq_t sum   = (seq_t)0;

    for (idx_t j = 0; j <= col; ++j) {
        seq_t comb = binom( 2 * col + 1, 2 * j + 1 );
        seq_t term = pow_c;
        term *= pow_d;
        term *= comb;
        sum += term;

        if (j < col) {
            pow_c /= c;               /* move to next lower power of c   */
            pow_d *= d;               /* move to next power of d        */
        }
    }
    return sum;
}
