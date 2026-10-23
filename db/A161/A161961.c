/*
 * A161961  Denominator of ratio in lowest terms of pi(n^2)/(n*pi(n)), where
 *          pi(x) = A000720(x).
 * https://oeis.org/A161961
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, frac, less
 * Formula:   a(n) = denominator of ( pi(n^2) / (n * pi(n)) ) in lowest terms
 * Range:     u8 n <= 28, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   compute_pi, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/compute_pi.h"

/* Return the denominator of pi(n^2)/(n*pi(n)) in lowest terms. */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0; /* Offset starts at 2; below that the value is undefined. */

    /* Compute prime counts. */
    seq_t pi_n      = compute_pi(n);
    idx_t n_sq     = (idx_t)n * (idx_t)n;
    seq_t pi_n_sq   = compute_pi(n_sq);

    /* Build numerator and denominator of the fraction. */
    seq_t num = pi_n_sq;
    seq_t den = (seq_t)n * pi_n;

    if (num == 0)    /* sanity: pi(n^2) never zero for n>=2, but keep safe */
        return 1;

    /* Reduce fraction and return the denominator. */
    seq_t g = gcd(num, den);
    return den / g;
}
