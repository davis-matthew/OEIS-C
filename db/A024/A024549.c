/*
 * A024549  Sum of [ 1/{k*sqrt(3)} ], k = 1,2,...,n, where {x} := x - [ x ].
 * https://oeis.org/A024549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..n} ⌊ 1 / {k·√3} ⌋ Using the identity ⌊ 1 / {k·√3}
 *            ⌋ = ⌊ 2·⌊k·√3⌋ / ( 3k² – ⌊k·√3⌋² ) ⌋ which follows from (k·√3 –
 *            ⌊k·√3⌋)(k·√3 + ⌊k·√3⌋) = 3k² – ⌊k·√3⌋²
 * Range:     u8 n <= 4, u16 n <= 57, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We need the integer square‑root routine for arbitrary precision
   integers.  It is provided by the utilities. */
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;

    for (idx_t k = 1; k <= n; ++k) {
        /* k as a seq_t value.  */
        seq_t tk = (seq_t)k;

        /* Compute v = 3·k².  */
        seq_t v = (seq_t)3 * tk * tk;

        /* a_k = ⌊ k·√3 ⌋ = ⌊ √(3·k²) ⌋.  */
        seq_t a_k = floor_sqrt(v);

        /* d = 3k² – a_k² = (k·√3 – a_k)(k·√3 + a_k) > 0.  */
        seq_t d = v - a_k * a_k;

        /* term_k = ⌊ 2·a_k / d ⌋.  */
        seq_t term = ((seq_t)2 * a_k) / d;

        sum += term;
    }

    return sum;
}
