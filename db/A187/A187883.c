/*
 * A187883  Triangle by rows, A003983 * A000012 as infinite lower triangular
 *          matrices.
 * https://oeis.org/A187883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = S(r) - Σ_{i=1}^{pos-1} min(i, r-i+1, ceil(r/2)) * where r
 *            is the smallest integer with r*(r+1)/2 ≥ n, * pos = n -
 *            r*(r-1)/2 (1‑based column in that row) and * S(r) = r +
 *            ((r-1)/2)*(r/2)
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* ----------------------------------
       Find the row r such that T(r) ≥ n,
       where T(r) = r*(r+1)/2.
       ---------------------------------- */
    idx_t r = 1;
    unsigned __int128 cumul = 1;          /* cumulative count up to current row */
    while (cumul < n) {
        ++r;
        cumul += r;
    }

    /* Position in the row (1‑based) */
    idx_t prev = (unsigned __int128)(r - 1) * r / 2;  /* T(r-1) */
    idx_t pos   = n - prev;                           /* 1 … r */

    /* ----------------------------------
       Compute the first element of row r :
       S(r) = r + floor((r-1)/2) * floor(r/2)
       ---------------------------------- */
    seq_t S = (seq_t)r
            + ((seq_t)(r - 1) / 2) * ((seq_t)r / 2);

    /* ----------------------------------
       Sum the first pos-1 elements of the
       original A003983 row (values are
       min(i, r-i+1, ceil(r/2))).
       ---------------------------------- */
    idx_t center = (r >> 1) + (r & 1);          /* ceil(r/2) */
    seq_t prefix = 0;
    for (idx_t i = 1; i < pos; ++i) {
        idx_t vi = i;
        idx_t t  = r - i + 1;
        if (t < vi) vi = t;
        if (center < vi) vi = center;
        prefix += (seq_t)vi;
    }

    return S - prefix;
}
