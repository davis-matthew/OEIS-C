/*
 * A169381  Number of reduced words of length n in Coxeter group on 32
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^31 = I.
 * https://oeis.org/A169381
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 32, a(n) = 992 · 31^(n‑2) for n ≥ 2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence first grows as 1, 32, 992, 30752, 953312, …  (see OEIS A169381)*/

#include "../utils/seq_pow.h"   /* provides seq_pow(seq_t, idx_t) */
#include "../utils/seq_mul.h"   /* provides seq_mul(seq_t, seq_t)   */

seq_t a(idx_t n)
{
    /* Very small indices: return the hard‑coded terms directly */
    if (n == 0U)
        return (seq_t)1;
    if (n == 1U)
        return (seq_t)32;

    /* For n ≥ 2:  a(n) = 992 × 31^(n‑2)  */
    seq_t pow_31 = seq_pow((seq_t)31, (idx_t)(n - 2U));
    return seq_mul(pow_31, (seq_t)992);
}
