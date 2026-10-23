/*
 * A160902  Square array read by antidiagonals: a(m,n) = the smallest prime >=
 *          m*n.
 * https://oeis.org/A160902
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = smallest prime >= (i+1)*(j+1) where (i,j) are
 * the 0‑based antidiagonal coordinates of n (n starts at 1).
 */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* out of range */

    /* Find the diagonal d such that the d‑th triangular number
       Tri(d) = d*(d+1)/2 is >= n. */
    idx_t d = 1;
    idx_t tri = 1;                 /* Tri(1) */
    while (tri < n) {
        ++d;
        tri += d;                  /* Tri(d) = Tri(d-1)+d */
    }

    /* Triangular number of previous diagonal */
    idx_t tri_prev = tri - d;
    /* 0‑based offset inside the current diagonal */
    idx_t offset = n - tri_prev - 1;

    /* Coordinates (i,j) in 0‑based form: i = offset,
       j = d - offset - 1. */
    idx_t i = offset;
    idx_t j = d - offset - 1;

    /* Compute product m*n = (i+1)*(j+1). */
    seq_t prod = (seq_t)(i + 1) * (seq_t)(j + 1);

    /* Find the smallest prime >= prod. */
    seq_t candidate = prod;
    if (candidate < 2) candidate = 2;          /* handle prod == 1 */
    while (!is_prime_big(candidate))
        ++candidate;

    return candidate;
}
