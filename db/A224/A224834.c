/*
 * A224834  a(n) = Sum {d|n, d <= n^(1/2)} tau(d)^2.
 * https://oeis.org/A224834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum{ d | n , d ≤ sqrt(n) } τ(d)^2 where τ(m) is the
 *            number of divisors of m. Implementation iterates over all
 *            divisors up to the integer square root
 * Range:     u8 n <= 719, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/divisor_count_u64.h" /* divisor_count_u64(uint64_t) */

seq_t a(idx_t n)
{
    /* n ≥ 1 */
    seq_t sum = 0;

    /* Loop over d from 1 while d*d ≤ n (integer square root) */
    for (idx_t d = 1; (d * d) <= n; ++d) {
        if (n % d == 0) {
            /* τ(d) ≤ d for d ≥ 1, fits comfortably in seq_t */
            seq_t tau = (seq_t)divisor_count_u64(d);
            sum += tau * tau;
        }
    }
    return sum;
}
