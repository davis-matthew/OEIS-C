/*
 * A190054  a(n) = n + [n*r/s] + [n*t/s]; r=2, s=sin(Pi/3), t=csc(Pi/3).
 * https://oeis.org/A190054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 18, u32 n <= 4174, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A190054
   a(n) = n + ⌊4n/√3⌋ + ⌊4n/3⌋   (n ≥ 1, offset 1) */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t sn    = (seq_t)n;           /* n as seq_t */
    seq_t term2 = (sn * (seq_t)4u) / (seq_t)3u;   /* ⌊4n/3⌋ */

    /* Compute ⌊4n/√3⌋ via integer square root:
       ⌊4n/√3⌋ = ⌊(4n√3)/3⌋ = ⌊⌊√(3·(4n)²)⌋ / 3⌋
    */
    uint64_t m = (uint64_t)n * 4u;           /* m = 4n   (64‑bit to avoid overflow) */
    uint64_t t = 3u * m * m;                 /* t = 3·m² (safe for n up to ~10 000) */
    seq_t sqrt_t = floor_sqrt((seq_t)t);     /* ⌊√t⌋ = ⌊√(3·(4n)²)⌋ */
    seq_t term1  = sqrt_t / (seq_t)3u;       /* ⌊√t⌋/3 = ⌊4n/√3⌋ */

    return sn + term1 + term2;
}
