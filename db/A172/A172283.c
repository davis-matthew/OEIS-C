/*
 * A172283  (-9,11) Pascal triangle.
 * https://oeis.org/A172283
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, sign
 * Formula:   a(n) is obtained by concatenating the rows of the (-9,11) Pascal
 *            triangle T(n,k). The triangle satisfies T(0,0)=1 and for n≥1:
 *            T(n,k) = binom(n,k) – 10·binom(n‑1,k) + 10·binom(n‑1,k‑1) (k≥0)
 *            (binom given by the utility routine with signature
 *            `binom(idx_t,idx_t)`)
 * Range:     i8 n <= 38, i16 n <= 54, i32 n <= 54, i64 n <= 54, i128 n <= 54,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Handle the first term directly */
    if (n == 0u)
        return (seq_t)1;

    /* Determine row r and column c such that 
       triangular(r) ≤ n < triangular(r+1),
       where triangular(r) = r*(r+1)/2. */
    idx_t r = 0u;
    idx_t cum = 0u;                 /* cumulative count of entries */
    while (n - cum >= r + 1u) {     /* use subtraction to avoid wrap */
        ++r;
        cum += r;
    }
    idx_t c = n - cum;              /* column index within row r */

    /* Compute T(r,c) via its closed‑form using binomial coefficients */
    seq_t term = binom(r, c);                   /* C(r,c) */
    if (c <= r - 1u)                            /* subtract 10·C(r-1,c) */
        term -= ((seq_t)10) * binom(r - 1u, c);
    if (c > 0u)                                 /* add 10·C(r-1,c-1) */
        term += ((seq_t)10) * binom(r - 1u, c - 1u);

    return term;
}
