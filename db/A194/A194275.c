/*
 * A194275  Concentric pentagonal numbers of the second kind: a(n) =
 *          floor(5*n*(n+1)/6).
 * https://oeis.org/A194275
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(5*n*(n+1)/6)
 * Range:     u8 n <= 17, u16 n <= 279, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic to avoid intermediate overflow. */
    __int128 v = (__int128)n * (n + 1);
    v /= 2;          /* n*(n+1) is even, division exact */
    v *= 5;
    v /= 3;          /* floor division */
    return (seq_t)v;
}
