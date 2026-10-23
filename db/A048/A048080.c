/*
 * A048080  Number of nonempty subsets of {1,2,...,n} in which exactly 1/6 of
 *          the elements are <= (n+2)/3.
 * https://oeis.org/A048080
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{t≥1} C(floor((n+2)/3), t) * C(n-floor((n+2)/3), 5t)
 * Range:     u8 n <= 12, u16 n <= 22, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*    for t such that t ≤ floor((n+2)/3), 5t ≤ n - floor((n+2)/3), and 6t ≤ n */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    /* n is at least 1; result is 0 for small n automatically */
    idx_t A = (n + 2) / 3;            /* number of elements ≤ (n+2)/3 */
    idx_t B = n - A;                  /* number of elements >  (n+2)/3 */

    /* number of t satisfying all constraints */
    idx_t max_t = A;
    if (B / 5 < max_t) max_t = B / 5;
    if (n / 6 < max_t) max_t = n / 6;

    seq_t result = 0;
    for (idx_t t = 1; t <= max_t; ++t) {
        seq_t c1 = binomial(A, t);          /* C(A, t)   */
        seq_t c2 = binomial(B, 5 * t);      /* C(B, 5t)  */
        result += c1 * c2;
    }
    return result;
}
