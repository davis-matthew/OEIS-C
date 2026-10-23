/*
 * A047444  Numbers that are congruent to {0, 3, 5, 6} mod 8.
 * https://oeis.org/A047444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Sequence A047444: numbers congruent to {0,3,5,6} modulo 8.
   a(1) = 0, a(2) = 3, a(3) = 5, a(4) = 6, a(5) = 8, … */
seq_t a(idx_t n)
{
    /* for n >= 1; behaviour for n < 1 is undefined (ignored). */
    seq_t m = (seq_t)(n - 1);          /* zero‑based index            */
    seq_t q = m / 4;                  /* number of complete 8‑blocks */
    seq_t r = m % 4;                  /* position within a block     */
    static const seq_t R[4] = { (seq_t)0, (seq_t)3, (seq_t)5, (seq_t)6 };
    return 8 * q + R[(size_t)r];
}
