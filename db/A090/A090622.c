/*
 * A090622  Square array read by antidiagonals of highest power of k dividing
 *          n! (with n,k>1).
 * https://oeis.org/A090622
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = exponent of k dividing n! where (n,k) are taken by
 *            reading the infinite square array of the largest power of k
 *            dividing n! along anti‑diagonals starting at (n,k) = (2,2). The
 *            array is read by increasing sums s = n+k; for a given s the
 *            pairs (n,k) with n≥2,k≥2 and n+k = s appear in the orde...
 * Range:     u8 n <= 8929, u16 n <= 8929, u32 n <= 8929, u64 n <= 8929,
 *            u128 n <= 8929, big any n
 * Helpers:   exp_k_in_factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/exp_k_in_factorial.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                     /* values below the offset are irrelevant */

    /* Find the anti‑diagonal sum s and the position within that sum. */
    idx_t m = n - 2;                   /* zero‑based index after the first term */
    idx_t s = 4;                       /* first sum: 2+2 */
    idx_t cum = 1;                     /* terms up to the current sum s */

    while (cum <= m) {
        ++s;
        cum += s - 3;                  /* number of terms in sum s */
    }

    idx_t cum_before = cum - (s - 3);  /* total terms before this sum */
    idx_t p = m - cum_before;          /* position within current sum, 0‑based */
    idx_t row = 2 + p;                 /* n in T(n,k) */
    idx_t col = s - row;               /* k in T(n,k) */

    return exp_k_in_factorial(row, col);
}
