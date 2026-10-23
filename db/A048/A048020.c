/*
 * A048020  Number of nonempty subsets of {1,2,...,n} in which exactly 3/4 of
 *          the elements are <= (n-2)/3.
 * https://oeis.org/A048020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{t>=1, 4t<=n, 3t<=floor((n-2)/3),
 *            t<=n-floor((n-2)/3)} binomial(floor((n-2)/3), 3t) *
 *            binomial(n-floor((n-2)/3), t)
 * Range:     u8 n <= 19, u16 n <= 31, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count of nonempty subsets of {1,…,n} containing exactly 3/4 of its elements in the first floor((n-2)/3) positions. */

#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    if (n < 2) return 0;                         /* for n = 1, result is 0 */

    /* m = floor((n-2)/3) */
    idx_t m = (n - 2) / 3;
    idx_t max_t = n / 4;                          /* because |S| = 4t */
    seq_t total = 0;

    for (idx_t t = 1; t <= max_t; ++t) {
        if (3 * t > m) break;                     /* too many elements in the first part */
        if (t > n - m) break;                     /* not enough elements in the second part */
        seq_t c1 = binomial(m, 3 * t);
        seq_t c2 = binomial(n - m, t);
        total += c1 * c2;
    }

    return total;
}
