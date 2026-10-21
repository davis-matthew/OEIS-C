/*
 * A070514  Final digit of n^4: a(n) = n^4 mod 10.
 * https://oeis.org/A070514
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n^4 mod 10
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the last digit of n raised to the fourth power. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use a 64‑bit integer for intermediate arithmetic
       to avoid overflow on small seq_t types. */
    uint64_t r = n % 10;          /* last decimal digit of n */
    uint64_t res = r * r * r * r; /* r^4 */
    res %= 10;                    /* reduce modulo 10 */
    return (seq_t)res;
}
