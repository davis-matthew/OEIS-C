/*
 * A189381  a(n) = n + [n*r/s] + [n*t/s]; r=1, s=-1+sqrt(2), t=1+sqrt(2).
 * https://oeis.org/A189381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊n·r/s⌋ + ⌊n·t/s⌋ with * r = 1, s = -1+√2, t = 1+√2.
 *            * Simplifying: a(n) = 5n + ⌊n·√2⌋ + ⌊2n·√2⌋
 * Range:     u8 n <= 2, u16 n <= 45, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We use the generic big‑integer utilities supplied by the build. */
#include "../utils/floor_sqrt.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type. */
    seq_t nn = (seq_t)n;

    /* nn² */
    seq_t n2 = seq_mul(nn, nn);

    /* floor(n·√2) = floor(√(2 n²)) */
    seq_t floor_ns2 = floor_sqrt(seq_mul(n2, (seq_t)2));

    /* floor(2n·√2) = floor(√(8 n²)) */
    seq_t floor_2ns2 = floor_sqrt(seq_mul(n2, (seq_t)8));

    /* Return a(n) = 5n + floor(n√2) + floor(2n√2). */
    return (seq_t)((seq_t)5 * nn + floor_ns2 + floor_2ns2);
}
