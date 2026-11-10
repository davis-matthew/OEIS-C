/*
 * A087136  Smallest positive number m such that A073642(m)=n.
 * https://oeis.org/A087136
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest m>0 with A073642(m)=n; greedy: repeatedly take
 *            smallest k with k(k+1)/2 >= r, set bit k, r -= k (a(0)=1)
 * Range:     u8 n <= 28, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 53 terms through n = 52 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t m = 0;
    idx_t r = n;
    if (n == 0)
        return (seq_t)1;
    while (r > 0) {
        idx_t k = 0;
        while (k * (k + 1) / 2 < r)
            k++;
        m |= ((seq_t)1 << (unsigned)k);
        r -= k;
    }
    return m;
}
