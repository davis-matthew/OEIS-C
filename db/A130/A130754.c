/*
 * A130754  A folded back triangular sequence of the binomial / Pascal's
 *          triangle A007318: half of the sequence is taken and doubled except
 *          for the odd middle terms which remain the same.
 * https://oeis.org/A130754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the flattened sequence obtained by folding back the
 *            first half of each row of Pascal's triangle. For row r (r>=0)
 *            let L(r) = floor(r/2)+1 be the number of elements kept. For r>0
 *            the elements are: a(r,0) = 2 a(r,i) = binomial(r,i) if r is even
 *            a(r,i) = 2 * binomial(r,i) if r is odd The ove...
 * Range:     u8 n <= 24, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Special case: first term */
    if (n == 1) return 1;

    /* Find the row r that contains the n-th element */
    idx_t r = 0;
    idx_t cumulative = 1;               /* cumulative count up to row r */

    while (cumulative < n) {
        ++r;
        cumulative += (r / 2) + 1;      /* length of row r */
    }

    /* Compute the offset within the row (0‑based) */
    idx_t row_len = (r / 2) + 1;
    idx_t prev_cumulative = cumulative - row_len;
    idx_t offset = n - prev_cumulative - 1;

    /* Evaluate the element */
    if (r == 0) return 1;              /* only element of row 0 */
    if (offset == 0) return 2;         /* first element of every row >0 */

    seq_t val = binomial(r, offset);
    if (r % 2 == 1)                    /* odd row: double all but the first */
        val = val * 2;

    return val;
}
