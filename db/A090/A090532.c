/*
 * A090532  Let f(n) = n - pi(n) = A062298(n). Then a(n) = least number of
 *          steps such that f(f(...(n))) = 1.
 * https://oeis.org/A090532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of iterations of f(x) = x - π(x) until reaching 1
 * Range:     u8 n <= 728, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compute_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    seq_t current = (seq_t)n;
    seq_t steps   = 0;

    while (current > (seq_t)1)
    {
        seq_t pi_val = compute_pi((idx_t)current);
        current -= pi_val;
        ++steps;
    }
    return steps;
}
