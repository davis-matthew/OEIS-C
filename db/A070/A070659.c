/*
 * A070659  a(n) = n^6 mod 36.
 * https://oeis.org/A070659
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^6 mod 36
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS sequence A070659,
   where a(n) = n^6 (mod 36).  The sequence
   is periodic with period 6: a(n) = a(n-6). */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* reduce n modulo 36 first */
    idx_t n_mod = n % 36;
    seq_t x = (seq_t)n_mod;      /* current value (mod 36) */
    seq_t res = 1;               /* accumulator for x^6 */

    /* multiply six times, reducing modulo 36 each step */
    for (int i = 0; i < 6; ++i)
        res = (res * x) % (seq_t)36;

    return res;
}
