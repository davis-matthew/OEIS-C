/*
 * A134081  Triangle T(n, k) = binomial(n, k)*((2*k+1)*(n-k) +k+1)/(k+1), read
 *          by rows.
 * https://oeis.org/A134081
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r, k satisfy r*(r+1)/2 ≤ n < (r+1)*(r+2)/2,
 *            k = n – r*(r+1)/2 T(r,k) =
 *            binom(r,k)*(((2*k+1)*(r-k)+k+1)/(k+1)) (OEIS A134081)
 * Range:     u8 n <= 30, u16 n <= 125, u32 n <= 478, u64 n <= 1979,
 *            u128 n <= 5150, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"

/* Return the row r such that offset r*(r+1)/2 ≤ n */
static idx_t row_for_n(idx_t n)
{
    idx_t r = 0;
    while (1)
    {
        /* Compute offset = r*(r+1)/2 */
        idx_t offset = r * (r + 1) / 2;
        if (n < offset + r + 1)     /* n lies in this row */
            return r;
        ++r;
    }
}

/* Compute a(n) */
seq_t a(idx_t n)
{
    idx_t r = row_for_n(n);
    idx_t offset = r * (r + 1) / 2;
    idx_t k = n - offset;

    seq_t bin = binom(r, k);

    /* Compute ((2*k+1)*(r-k)+k+1) */
    seq_t num = ((seq_t)(2 * k + 1)) * (seq_t)(r - k) + (seq_t)(k + 1);
    seq_t denom = (seq_t)(k + 1);

    /* Use 128‑bit intermediate to avoid overflow */
    unsigned __int128 prod = (unsigned __int128)bin * (unsigned __int128)num;
    prod /= (unsigned __int128)denom;

    return (seq_t)prod;
}
