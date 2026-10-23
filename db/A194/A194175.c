/*
 * A194175  [sum{(k*r) : 1<=k<=n}], where [ ]=floor, ( )=fractional part, and
 *          r=sqrt(7).
 * https://oeis.org/A194175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( sqrt(7) * n*(n+1)/2 ) – Σ_{k=1}^{n}
 *            floor(k*sqrt(7))
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*           = floor( sqrt(7) * n*(n+1)/2 ) – Σ_{k=1}^{n} floor( sqrt(7*k*k) ) */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

/* Return the n-th term of OEIS A194175. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* triangular number T = n*(n+1)/2, computed using idx_t first. */
    idx_t T = n * (n + 1) / 2;

    /* floor( sqrt(7) * T )  = floor( sqrt(7*T^2) ) */
    seq_t top_val = (seq_t)7 * (seq_t)T * (seq_t)T;
    seq_t top = floor_sqrt(top_val);

    /* Σ floor( sqrt(7*k^2) ) for k = 1..n */
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t v = (seq_t)7 * (seq_t)k * (seq_t)k;
        sum += floor_sqrt(v);
    }

    return top - sum;
}
