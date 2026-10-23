/*
 * A173043  Triangle T(n, k, q) = binomial(n, k) - 1 + q^(n*binomial(n-2,
 *          k-1)) with T(n, 0, q) = T(n, n, q) = 1 and q = 2, read by rows.
 * https://oeis.org/A173043
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(n, k, 2) read by rows, where T(n, k, 2) = binomial(n,
 *            k) - 1 + 2^( n * binomial(n-2, k-1) ) with T(n, 0, 2) = T(n, n,
 *            2) = 1
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 23, u64 n <= 30, u128 n <= 39,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The routine finds the row r and column k that contain the
   n‑th term in the triangular array, then evaluates the
   formula using the helper functions provided in the library. */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* --- locate row r  --- */
    idx_t r = 0;
    /* cumulative number of terms up to row r: (r+1)(r+2)/2   */
    while ( ((( __int128) (r + 1)) * (r + 2)) / 2 <= n )
        ++r;

    /* number of terms before row r */
    const idx_t prev = ( ( (__int128) r * (r + 1) ) / 2 );
    const idx_t k    = n - prev;      /* column within the row, 0‑based */

    /* ---------- end values ------------- */
    if (k == 0 || k == r)
        return (seq_t)1;

    /* ---------- interior value ------------- */
    seq_t binom1 = binomial(r, k);
    seq_t term1  = binom1 - (seq_t)1;

    /* exponent part: r * binomial(r-2, k-1) */
    seq_t binom2 = binomial(r - 2, k - 1);
    seq_t exp    = (seq_t)r * binom2;

    seq_t part2 = big_pow((seq_t)2, exp);
    return term1 + part2;
}
