/*
 * A174045  Triangle T(n, k, q) = Sum_{j=0..10} q^j * floor(
 *          binomial(n+1,k)*binomial(n-1,k-1)/(2^j*(n+1)) ) for q = 3, read by
 *          rows.
 * https://oeis.org/A174045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy, less
 * Formula:   a(n) = Sum_{j=0..10} 3^j * floor( binom(n+1,k) * binom(n-1,k-1)
 *            / * (2^j * (n+1)) ), where (n,k) are the row‑column *
 *            coordinates of the n‑th element when the triangle * T(n,k) is
 *            read row by row. Offset = 1
 * Range:     u8 n <= 17, u16 n <= 39, u32 n <= 143, u64 n <= 576,
 *            u128 n <= 1275, big any n
 * Helpers:   binom, col_of_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"
#include "../utils/col_of_idx.h"

/* Return the row (n) of the triangle that contains the t‑th element
 * when the triangle is read by rows.  */
static idx_t row_of_idx(idx_t t)
{
    idx_t r = 1;
    while ( ((unsigned __int128)r * (r + 1)) / 2 < t )
        ++r;
    return r;
}

/* Compute T(r,k) for a fixed row r and column k.  */
static seq_t T_row_col(idx_t r, idx_t k)
{
    seq_t bin1 = binom(r + 1, k);          /* binom(r+1,k)   */
    seq_t bin2 = binom(r - 1, k - 1);      /* binom(r-1,k-1) */

    /* product of the two binomials.  Use 128‑bit to avoid overflow.  */
    unsigned __int128 prod = (unsigned __int128)bin1 *
                            (unsigned __int128)bin2;

    unsigned __int128 acc   = 0;          /* accumulator (128‑bit) */
    unsigned int      pow3  = 1;          /* current 3^j (fits in 32‑bit) */

    for (idx_t j = 0; j <= 10; ++j) {
        unsigned __int128 denom = ((unsigned __int128)(r + 1)) << j; /* 2^j */
        unsigned __int128 term  = prod / denom;                     /* floor */
        acc += term * (unsigned __int128)pow3;                       /* add q^j * term */

        pow3 *= 3;                                                  /* next 3^j */
    }

    return (seq_t)acc;   /* truncate if seq_t is smaller than 128‑bit */
}

/* The public sequence function required by the harness.  */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* Offset 1 – the value for n = 0 is unused.  */

    idx_t r = row_of_idx(n);
    idx_t k = col_of_idx(n, r);

    return T_row_col(r, k);
}
