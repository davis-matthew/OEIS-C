/*
 * A194151  Beatty sequence for (1/2)*sqrt(5); complement of A194152.
 * https://oeis.org/A194151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊n * √5 / 2⌋ (Beatty sequence for (1/2)·√5)
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Compute floor(√(5*n*n)) and then take integer division by 2. */
    seq_t nn   = (seq_t)n * (seq_t)n;   /* n² */
    seq_t prod = (seq_t)5 * nn;         /* 5·n² */
    seq_t s    = floor_sqrt(prod);      /* ⌊√(5·n²)⌋ = ⌊n·√5⌋ */
    return s / 2;                       /* ⌊⌊n·√5⌋ / 2⌋ = desired value */
}
