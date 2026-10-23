/*
 * A140822  Triangle T(n,m) = binomial(n,gcd(n,m)) read by rows, 0<=m<=n.
 * https://oeis.org/A140822
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(⌊r⌋, m) where T(r,m) = binomial(r, gcd(r, m)) and the
 *            terms are read row‑wise from the triangle T(n, m) = binomial(n,
 *            gcd(n, m)), 0 <= m <= n
 * Range:     u8 n <= 39, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence a(n) defined in OEIS A140822 is obtained by
   flattening that triangle row‑by‑row.  This implementation
   first determines which row r the index n falls into, then
   finds the column offset m, computes gcd(r, m) and finally
   evaluates binomial(r, gcd(r, m)).  All arithmetic is performed
   in seq_t, and standard helpers are used only where appropriate. */

#include "../utils/binomial.h"

static idx_t gcd_idx(idx_t a, idx_t b)
/* Euclidean algorithm for idx_t (unsigned 64‑bit). */
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
/* Returns the nth term (0‑based) of the flattened triangle. */
{
    /* Find the row r such that n is in the range
       [row_start, row_start + r] where row_start = r*(r+1)/2. */
    idx_t r = 0;
    idx_t cumulative = 0;               /* total terms before current row */
    while (cumulative + (r + 1) <= n) {
        cumulative += (r + 1);
        ++r;
    }
    /* Offset within the row */
    idx_t m = n - cumulative;           /* 0 <= m <= r */

    /* Compute gcd(r, m) */
    idx_t g = gcd_idx(r, m);

    /* Return binomial(r, g) in seq_t */
    return binomial(r, g);
}
