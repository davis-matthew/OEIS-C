/*
 * A015118  Triangle of q-binomial coefficients for q=-8.
 * https://oeis.org/A015118
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   not stated
 * Range:     i8 n <= 6, i16 n <= 15, i32 n <= 23, i64 n <= 38, i128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A015118 – Triangle of q‑binomial coefficients for q = –8.
    a(n) is the (n+1)‑st element of the triangle read row‑by‑row
    (row indices r = ⌊½(√(8n+1) – 1)⌋, column k = n – r(r+1)/2),
    and the value is the Gaussian binomial coefficient
                    ⟨r k⟩_{q=‑8} . */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the row r containing the index n. */
    idx_t r = 0;
    while ((r * (r + 1) / 2) <= n) ++r;
    --r;                                /* now r satisfies r(r+1)/2 ≤ n < (r+1)(r+2)/2 */

    idx_t row_start = r * (r + 1) / 2;   /* first index of row r */
    idx_t k = n - row_start;             /* column index within the row */

    /* Use symmetry: ⟨r k⟩ = ⟨r r–k⟩.  Keep k ≤ r/2 for efficiency. */
    idx_t kk = k;
    if (kk > r - kk) kk = r - kk;

    seq_t result = (seq_t)1;
    if (kk == 0) return result;          /* trivial case */

    /* Pre‑compute powers of q = –8 that will be needed.  The
       calculation keeps track of the current numerator and
       denominator powers to avoid recomputing powers repeatedly. */
    const seq_t q   = (seq_t)-8;
    const seq_t one = (seq_t)1;

    /* numer_pow holds q^(r–i+1) during the loop; initially q^r. */
    seq_t numer_pow = (seq_t)1;
    for (idx_t i = 0; i < r; ++i) numer_pow *= q;      /* q^r */

    /* denom_pow holds q^i during the loop; initially q^1. */
    seq_t denom_pow = q;                                 /* q^1 */

    for (idx_t i = 1; i <= kk; ++i) {
        seq_t numer = one - numer_pow;      /* 1 – q^(r–i+1) */
        seq_t denom = one - denom_pow;      /* 1 – q^i     */
        result = result * numer / denom;    /* exact division */
        numer_pow /= q;                     /* advance to q^(r–i)   */
        denom_pow *= q;                     /* advance to q^(i+1)   */
    }
    return result;
}
