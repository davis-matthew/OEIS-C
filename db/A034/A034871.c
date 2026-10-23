/*
 * A034871  Odd-numbered rows of Pascal's triangle.
 * https://oeis.org/A034871
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A034871 – odd‑numbered rows of Pascal's triangle.
   For each even row r (except r = 0) the sequence outputs two 1's.
   For each odd row r the sequence outputs all inner binomial coefficients
   C(r, k) with k = 1 … r–1 (i.e. the coefficients except the outer 1's).
   Row 0 contributes a single 1. */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    idx_t count = 0;   /* number of terms produced so far          */
    idx_t r = 0;       /* current row of Pascal's triangle         */
    while (1)
    {
        idx_t add;
        if (r == 0)
            add = 1;                       /* row 0 -> one 1        */
        else if ((r & 1) == 0)
            add = 2;                       /* even rows (r>0) -> 1,1 */
        else
            add = r - 1;                   /* odd rows   -> r–1 inner terms */

        if (count + add > n)               /* target falls in this row */
        {
            idx_t pos = n - count;         /* position within the row  */

            if (r == 0 || ((r & 1) == 0))
                return (seq_t)1;           /* outer 1's                */

            /* odd row, inner coefficients */
            idx_t k = pos + 1;             /* k ranges 1 … r‑1         */
            return binom(r, k);            /* exact binomial coeff.     */
        }

        count += add;
        ++r;
    }
}
