/*
 * A173741  T(n,k) = binomial(n,k) + 4 for 1 <= k <= n - 1, n >= 2, and T(n,0)
 *          = T(n,n) = 1 for n >= 0, triangle read by rows.
 * https://oeis.org/A173741
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = value of the triangle T(n,k) read row‑by‑row where
 *            T(n,0)=T(n,n)=1 and (for 1 ≤ k ≤ n−1) T(n,k)=C(n,k)+4. a(0) = 1;
 *            subsequent terms follow the pattern described above
 * Range:     u8 n <= 59, u16 n <= 197, u32 n <= 646, u64 n <= 2376,
 *            u128 n <= 5150, big any n
 * Helpers:   binomial_generic
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/binomial_generic.h"

/* Main function required by the framework. */
seq_t a(idx_t n)
{
    /* Find the row r such that the first element of row r
       has index cum = r(r+1)/2 <= n < r(r+1)/2 + (r+1). */
    idx_t r   = 0;
    idx_t cum = 0;

    while (cum + r + 1 <= n) {
        cum += r + 1;
        ++r;
    }

    /* Position of the element within row r */
    idx_t c = n - cum;

    /* Boundary values are 1; interior values are binomial + 4. */
    if (c == 0 || c == r) {
        return (seq_t)1;
    } else {
        return binomial_generic(r, c) + (seq_t)4;
    }
}
