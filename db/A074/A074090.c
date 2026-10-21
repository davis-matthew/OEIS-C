/*
 * A074090  Number of steps to reach an integer starting with s = n/4 and
 *          iterating the map x -> s*ceiling(x).
 * https://oeis.org/A074090
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of iterations of xₖ₊₁ = (n/4) * ceil(xₖ) required
 *            to make the value integral, starting from x₀ = n/4. The
 *            recurrence stops as soon as n/4 * tₖ becomes an integer, where
 *            tₖ is the integer produced by the ceil operation at step k
 * Range:     u8 n <= 10, u16 n <= 74, u32 n <= 107, u64 n <= 107,
 *            u128 n <= 107, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* For n < 5 the sequence is undefined; return 0. */
    if (n < 5) {
        return 0;
    }

    /* Determine the reduced denominator d = 4 / gcd(n,4). */
    idx_t g = 1;
    if ((n % 4) == 0) {
        g = 4;
    } else if ((n % 2) == 0) {
        g = 2;
    } else {
        g = 1;
    }
    seq_t d = (seq_t)(4 / g);
    if (d == 1) {
        /* n divisible by 4 → already integer. */
        return 0;
    }

    seq_t t = 1;      /* current integer in the iteration: t₀ = 1 */
    seq_t steps = 0;  /* number of iterations performed */

    while ((t % d) != 0) {
        /* ceil((n * t) / 4) = (n * t + 3) / 4 for positive integers. */
        seq_t prod = (seq_t)n * t;
        t = (prod + 3) / 4;
        ++steps;
    }
    return steps;
}
