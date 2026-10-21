/*
 * A070495  a(n) = n^3 mod 33.
 * https://oeis.org/A070495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 mod 33
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Reduce n modulo 33 first to keep intermediate results small. */
    idx_t r   = n % 33;            /* r == n mod 33   */
    idx_t sq  = (r * r) % 33;      /* r^2 mod 33      */
    idx_t cube = (sq * r) % 33;    /* r^3 mod 33      */
    return (seq_t)cube;
}
