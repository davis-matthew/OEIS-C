/*
 * A059344  Triangle read by rows: row n consists of the nonzero coefficients
 *          of the expansion of 2^n x^n in terms of Hermite polynomials with
 *          decreasing subscripts.
 * https://oeis.org/A059344
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, tabf
 * Formula:   a(r,k) = r!/(k! (r‑2k)!) (row r, position k, read by rows)
 * Range:     u8 n <= 17, u16 n <= 33, u32 n <= 87, u64 n <= 221,
 *            u128 n <= 621, big any n
 * Helpers:   binomial, factorial, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/factorial.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* Find the row r containing the nth entry and its offset k. */
    idx_t r   = 0;
    idx_t cum = 0;                       /* number of entries before row r */
    while (1) {
        idx_t len = (r >> 1) + 1;        /* floor(r/2)+1 entries in row r */
        if (cum + len > n)
            break;
        cum += len;
        ++r;
    }
    idx_t k = n - cum;                   /* position within row r */

    /* Compute a(r,k) = r!/(k! (r-2k)!) */
    seq_t term1 = binomial(r, k);         /* r!/(k!(r-k)!) */
    seq_t term2 = binomial(r - k, k);    /* (r-k)!/(k!(r-2k)!) */
    seq_t factk = factorial(k);           /* k! */

    seq_t prod = seq_mul(term1, term2);   /* r!/(k!^2 (r-2k)!) */
    return seq_mul(prod, factk);         /* multiply by k! to get correct value */
}
