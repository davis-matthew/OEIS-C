/*
 * A189378  a(n) = n + [nr/s] + [nt/s]; r=2, s=(-1+sqrt(5))/2,
 *          t=(1+sqrt(5))/2.
 * https://oeis.org/A189378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊(2·φ)·n⌋ + ⌊φ²·n⌋ Equivalent integer form: a(n) =
 *            3·n + ⌊n·√5⌋ + ⌊(n + ⌊n·√5⌋)/2⌋ (φ = (1+√5)/2)
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    /* guard for offset handling – a(0) is undefined, return 0 */
    if (n == 0) return 0;

    seq_t nseq = (seq_t)n;
    /* ⌊n·√5⌋ = ⌊√(5·n²)⌋ */
    seq_t sqrt5_part = floor_sqrt(((seq_t)5) * nseq * nseq);

    seq_t triple_n = nseq * 3;          /* 3·n */
    seq_t sum1 = triple_n + sqrt5_part; /* first part */

    seq_t sum2 = (nseq + sqrt5_part) / 2; /* ⌊(n+⌊n√5⌋)/2⌋ */

    return sum1 + sum2;
}
