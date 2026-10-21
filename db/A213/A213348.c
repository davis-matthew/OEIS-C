/*
 * A213348  6-quantum transitions in systems of N >= 6 spin 1/2 particles, in
 *          columns by combination indices.
 * https://oeis.org/A213348
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = 2^(N-6-2k) * binomial(N,k) * binomial(N-k,6+k) where N
 *            and k are determined by flattening the triangle for rows N =
 *            6,7,8,… and columns k = 0..floor((N-6)/2)
 * Range:     u8 n <= 9, u16 n <= 21, u32 n <= 63, u64 n <= 250,
 *            u128 n <= 1009, big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Minimum index is 6; below yield undefined values */
    if (n < 6) return 0;

    /* Offset into the flattened triangle (zero‑based) */
    idx_t offset = n - 6;

    /* Find the row N that contains this offset */
    idx_t N = 6;
    idx_t cumulative = 0;                 /* terms before the current row */
    while (1) {
        idx_t len = ((N - 6) / 2) + 1;    /* number of terms in this row */
        if (cumulative + len > offset)   /* this row contains the term */
            break;
        cumulative += len;
        N++;
    }

    /* Column index k within the found row */
    idx_t k = offset - cumulative;

    /* Compute the term using the formula */
    seq_t val = pow2(N - 6 - 2 * k);
    val *= binomial(N, k);
    val *= binomial(N - k, 6 + k);

    return val;
}
