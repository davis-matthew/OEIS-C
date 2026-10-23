/*
 * A141680  Triangle read by rows: T(n,m) = (n/m)*binomial(n,m) if m divides
 *          n, otherwise 0.
 * https://oeis.org/A141680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,m) = (n/m) * binomial(n,m) if m divides n, else 0 * a(n) is
 *            the linearization of the triangle by rows (n = row, m = column)
 * Range:     u8 n <= 31, u16 n <= 161, u32 n <= 577, u64 n <= 2311,
 *            u128 n <= 5050, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find the row `r` such that 1 + 2 + … + r ≥ n. */
    idx_t lo = 1, hi = n;       /* inclusive upper bound */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        unsigned __int128 cum = (unsigned __int128)mid * (mid + 1) / 2;
        if (cum >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t r = lo;  /* row number (1‑based) */

    /* Compute the column number `m`. */
    unsigned __int128 prev = (unsigned __int128)(r - 1) * r / 2;
    idx_t m = n - (idx_t)prev;   /* column number (1‑based) */

    /* If m does not divide r, the entry is zero. */
    if (r % m != 0)
        return (seq_t)0;

    /* Compute (n/m) * C(n,m). */
    seq_t bin = binomial(r, m);
    seq_t factor = (seq_t)(r / m);
    return bin * factor;
}
