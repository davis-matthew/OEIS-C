/*
 * A176348  Triangle, read by rows: T(n, k) = binomial(n, k)*(1 + 2*(n+1) -
 *          (k+1)*floor((n+1)/(k+1)) - (n-k+1)* floor((n+1)/(n-k+1))).
 * https://oeis.org/A176348
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n-th element of the triangle read by rows, where
 *            T(n,k) = binomial(n,k) * (1 + 2*(n+1) - (k+1)*floor((n+1)/(k+1))
 *            - (n-k+1)*floor((n+1)/(n-k+1)))
 * Range:     u8 n <= 38, u16 n <= 141, u32 n <= 479, u64 n <= 1983,
 *            u128 n <= 5150, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find the row (r) and column (k) that correspond to the flat index n. */
    idx_t r = 0;          /* current row */
    idx_t offset = 0;     /* number of elements before row r */

    /* Advance rows until the start of the next row would exceed n. */
    while (offset + r + 1 <= n) {
        offset += r + 1;  /* elements in the current row */
        ++r;
    }

    idx_t k = n - offset;  /* column within row r */

    /* Compute the binomial coefficient. */
    seq_t bin = binomial(r, k);

    /* Compute the multiplicative factor from the OEIS formula. */
    seq_t factor = (seq_t)1
                   + (seq_t)2 * (seq_t)(r + 1)
                   - (seq_t)(k + 1) * (seq_t)((r + 1) / (k + 1))
                   - (seq_t)(r - k + 1) * (seq_t)((r + 1) / (r - k + 1));

    return bin * factor;
}
