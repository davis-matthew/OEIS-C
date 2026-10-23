/*
 * A107111  Number array whose rows are the series reversions of
 *          x(1-x)/(1+x)^k, read by antidiagonals.
 * https://oeis.org/A107111
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = read by antidiagonals from triangle T(n,k) where T(n,k) =
 *            Σ_{j=0..k} binomial(n*(k+1), k-j) * binomial(k+j, j) / (k+1)
 * Range:     u8 n <= 17, u16 n <= 41, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"

/* Compute the entry T(n,k) of the triangle. */
static seq_t compute_T(idx_t n, idx_t k)
{
    seq_t sum = 0;
    idx_t N = n * (k + 1);
    for (idx_t j = 0; j <= k; ++j) {
        seq_t b1 = binomial(N, k - j);
        seq_t b2 = binomial(k + j, j);
        sum += b1 * b2;
    }
    return sum / (seq_t)(k + 1);
}

/* Return the n-th term of the sequence a. */
seq_t a(idx_t n)
{
    /* Find the antidiagonal d that contains the index n
       and the offset within that diagonal.  The total number
       of elements before diagonal d is d*(d+1)/2.  We iterate
       until that total exceeds n. */
    idx_t d = 0;
    idx_t prev = 0;          /* elements before current d */
    while (prev + d + 1 <= n) {
        ++d;
        prev += d;           /* now prev = d*(d+1)/2 */
    }

    idx_t offset = n - prev;   /* 0 ≤ offset ≤ d */
    idx_t row   = d - offset;  /* n = row + col, col = offset */
    idx_t col   = offset;

    return compute_T(row, col);
}
