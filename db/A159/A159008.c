/*
 * A159008  Positive numbers k such that k^2 == 2 (mod 89).
 * https://oeis.org/A159008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (89 + 11*(-1)^(n-1) + 178*(n-1))/4
 * Range:     u8 n <= 6, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* offset protection – behaviour below n=1 is irrelevant */
    long long m = (long long)n - 1;        /* (n‑1) as a signed value for the (-1)^(n-1) term */
    long long t = 89 + ((m & 1) ? -11 : 11) + 178 * m;
    return (seq_t)(t / 4);                 /* guaranteed to be integer */
}
