/*
 * A162996  a(n) = Round(kn * (log(kn)+1)), with k = 2.216 as an approximation
 *          of R_n = n-th Ramanujan prime A104272(n) and Abs(a(n)-R_n) < 2 *
 *          Sqrt(a(n)) for n in [1..1000].
 * https://oeis.org/A162996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Round(2.216 * n * (log(2.216 * n)+1))
 * Range:     u8 n <= 23, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

static const double K = 2.216;

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1. For n == 0 we return 0
       which is unused in OEIS. */
    if (n == 0)
        return 0;

    /* Compute the value using double precision (log base e). */
    double kn = K * (double)n;
    double val = kn * (log(kn) + 1.0);

    /* Round to nearest integer. */
    long long r = llround(val);

    return (seq_t)r;
}
