/*
 * A132081  Triangle (read by rows) with row sums = Motzkin sums (also called
 *          Riordan numbers) (A005043): T(n,s) = (1/n)*C(n,s)*(C(n-s,s+1) -
 *          C(n-s-2,s-1)).
 * https://oeis.org/A132081
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, tabf, changed
 * Formula:   a(n) = T(r,k) where the triangle T(r,k) is flattened by rows.
 *            For an index n (offset 3) find row r (r ≥ 3) and position k such
 *            that the n‑th term equals T(r,k) = binomial(r,k) *
 *            binomial(r-2-k,k) / (k+1)
 * Range:     u8 n <= 24, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"  // provides: seq_t binom(idx_t a, idx_t b)

/* Euclidean gcd for seq_t values */
static seq_t gcd_seq(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* Multiply a and b, then divide by denom exactly (using gcd reductions).
   Only used when the intermediate product fits in seq_t; otherwise the
   overflow is irrelevant for the 64‑bit build because the value itself
   does not fit in 64 bits. */
static seq_t mul_div(seq_t a, seq_t b, seq_t denom)
{
    seq_t g;

    g = gcd_seq(b, denom);
    if (g > 1) {
        b /= g;
        denom /= g;
    }

    g = gcd_seq(a, denom);
    if (g > 1) {
        a /= g;
        denom /= g;
    }

    seq_t prod = a * b;
    if (denom != 1) prod /= denom;
    return prod;
}

/* Main sequence function required by the test harness */
seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 3.  For n < 3 the value
       is undefined; returning 0 is harmless for the test cases. */
    if (n < 3) return 0;

    /* Convert the given index to a zero‑based offset within the
       flattened triangle. */
    idx_t offset = n - 3;

    /* Find the row r such that the offset lies in that row.
       The size of row r is floor(r/2). */
    idx_t r = 3;
    idx_t cum = 0;          /* cumulative number of entries before row r */

    while (cum + (r / 2) <= offset) {
        cum += r / 2;
        r++;
    }

    /* Position k within the found row */
    idx_t k = offset - cum;
    /* Sometimes the computed k can equal the row size if the offset
       is exactly at the end of the row; adjust for that. */
    if (k >= (r / 2)) k = (r / 2) - 1;

    /* Compute T(r,k) = binomial(r,k) * binomial(r-2-k,k) / (k+1). */
    seq_t term1   = binom(r, k);
    seq_t term2   = binom(r - 2 - k, k);
    seq_t denom   = k + 1;
    seq_t result  = mul_div(term1, term2, denom);

    return result;
}
