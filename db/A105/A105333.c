/*
 * A105333  a(n) = n*(n+1)/2 mod 16.
 * https://oeis.org/A105333
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)/2 mod 16, period 32
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Reduce the index modulo the period (32) to avoid large intermediate values
     * regardless of the underlying size of seq_t.
     */
    const idx_t PERIOD = 32;
    idx_t k = n % PERIOD;

    /* Cast to seq_t, compute the triangular number, then reduce modulo 16. */
    seq_t m = (seq_t)k;
    seq_t val = m * (m + 1) / 2;
    return val % (seq_t)16;
}
