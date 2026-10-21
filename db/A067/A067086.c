/*
 * A067086  a(n) = floor(Sum_{k=1..n} 1/k^(1/3)).
 * https://oeis.org/A067086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(∑_{k=1..n} 1/k^(1/3))
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    long double sum = 0.0L;
    for (idx_t k = 1; k <= n; ++k) {
        /* compute 1 / k^(1/3) using long double arithmetic */
        long double term = 1.0L / powl((long double)k, 1.0L / 3.0L);
        sum += term;
    }
    /* round down, adding a tiny epsilon to guard against round‑off errors */
    long long value = (long long)floorl(sum + 1e-12L);
    return (seq_t)value;
}
