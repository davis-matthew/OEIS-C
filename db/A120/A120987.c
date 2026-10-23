/*
 * A120987  Triangle read by rows: T(n,k) is the number of ternary words of
 *          length n with k strictly increasing runs (0 <= k <= n; for
 *          example, the ternary word 2|01|12|02|1|1|012|2 has 8 strictly
 *          increasing runs).
 * https://oeis.org/A120987
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 73, u32 n <= 266, u64 n <= 929,
 *            u128 n <= 3538, big any n
 * Helpers:   binom, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = number of ternary words of length r with k strictly
   increasing runs, where (r,k) is found by reading the triangle A120987
   row‑by‑row.  The coefficient is the trinomial coefficient

       T(r,k) = coefficient of x^{3k-r} in (1+x+x^2)^{r+1}.

   Formula implemented via the identity

       coeff(r+1, m) = Σ_{j=0..⌊m/2⌋} C(r+1, j) * C(r+1-j, m-2j),

   where m = 3k-r.  C denotes binomial coefficients.  The function
   uses the provided utility functions binom() and seq_mul() to
   guarantee exactness for all supported seq_t types. */

#include "../utils/binom.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* Find row r such that S(r-1) <= n < S(r), where
       S(r) = (r+1)(r+2)/2 is the number of entries up to row r. */
    idx_t r = 0;
    idx_t entries = 0;            /* S(r-1) */
    while ((entries + (r + 1)) <= n) {
        entries += (r + 1);      /* add current row length */
        ++r;
    }
    /* Now entries = S(r-1), and r is the row containing n. */
    idx_t k = n - entries;       /* column index within the row */

    /* Compute the trinomial coefficient. */
    idx_t len = r + 1;           /* exponent of (1+x+x^2) */
    idx_t m   = 3 * k - r;       /* target power in the expansion */

    if (m > 2 * len || (idx_t)m < 0) return (seq_t)0;

    seq_t sum = (seq_t)0;
    for (idx_t j = 0; j <= m / 2; ++j) {
        seq_t c1 = binom(len, j);
        seq_t c2 = binom(len - j, m - 2 * j);
        sum += seq_mul(c1, c2);
    }
    return sum;
}
