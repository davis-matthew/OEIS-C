/*
 * A131619  A general two modulo Ackermann recursion at 6 and 5.
 * https://oeis.org/A131619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, uned
 * Formula:   a(n) is the antidiagonal (reversed) transform of the *
 *            double-modulo Ackermann function defined by: * * a(1, n) = n mod
 *            6 * a(m, 1) = a(m-1, 2) * a(m, n) = a(m-1, a(m, n-1)+1) mod 5 *
 *            * The 1‑D sequence is obtained by enumerating the values *
 *            a(m,n) along the diagonals m+n = const with the order...
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   a_func
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a_func.h"

/* Public function implementing the OEIS sequence A131619. */
seq_t a(idx_t n)
{
    if (n < 1) {
        return 0; /* undefined below offset 1; return 0 as a sentinel */
    }

    /* Find the diagonal sum s such that the cumulative number of terms
     * up to that diagonal is at least n.  The cumulative number of terms
     * up to sum s is s*(s-1)/2.  Since n can be large, use 64‑bit
     * arithmetic; s never exceeds sqrt(2*n). */
    uint64_t s = 2;
    while ((s * (s - 1ull)) / 2 < n) {
        ++s;
    }

    /* Number of terms before the current diagonal. */
    uint64_t prev = (s - 1) * (s - 2) / 2;

    /* Zero‑based offset within the diagonal.  The diagonal is traversed
     * in reverse order: indices (m=n=s-1, n=1), (m=s-2, n=2), …   */
    uint64_t pos = n - prev - 1;

    seq_t m = (seq_t)(s - 1 - pos); /* row index */
    seq_t col = (seq_t)(pos + 1);   /* column index */

    return a_func(m, col);
}
