/*
 * A189524  a(n) = n + [n*s/r] + [n*t/r]; r=2, s=1+sqrt(2), t=1+sqrt(3).
 * https://oeis.org/A189524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊n·(1+√2)/2⌋ + ⌊n·(1+√3)/2⌋
 * Range:     u8 n <= 4, u16 n <= 73, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

/* Return the nth term of OEIS A189524 (offset 1). */
seq_t a(idx_t n)
{
    /* Convert the index to the sequence type. */
    seq_t nt = (seq_t)n;

    /* Compute ⌊n·√2⌋  and  ⌊n·√3⌋  via the integer square‑root. */
    seq_t n2      = nt * nt;            /* n²  (may overflow seq_t, but is reused below)   */
    seq_t prod2   = (seq_t)2 * n2;       /* 2·n²  */
    seq_t sqrt2n  = floor_sqrt(prod2);   /* ⌊√(2·n²)⌋ = ⌊n·√2⌋                     */
    seq_t prod3   = (seq_t)3 * n2;       /* 3·n²  */
    seq_t sqrt3n  = floor_sqrt(prod3);   /* ⌊√(3·n²)⌋ = ⌊n·√3⌋                     */

    /* ⌊n·(1+√k)/2⌋ = ⌊(n + ⌊n·√k⌋)/2⌋, because the fractional part of ⌊n·√k⌋/2 never
       changes the integer floor.                                                */
    seq_t part2 = (nt + sqrt2n) / (seq_t)2;
    seq_t part3 = (nt + sqrt3n) / (seq_t)2;

    /* Final value. */
    return nt + part2 + part3;
}
