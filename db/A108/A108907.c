/*
 * A108907  Number of times a point sum n is attained in all 6^6 permutations
 *          of throwing 6 dice.
 * https://oeis.org/A108907
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of (x + x^2 + ... + x^6)^6, i.e. number of
 *            6‑tuples (a1,…,a6) with ai∈{1,…,6} and a1+…+a6 = n. Computed by
 *            inclusion–exclusion: a(n) = Σ_{k=0}^{⌊(n-6)/6⌋} (-1)^k * C(6,k)
 *            * C(n-6-6k+5,5) for n≥6, a(n)=0 for n<6 or n>36
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n < 6 || n > 36) return 0;
    seq_t count = 0;
    idx_t s = n - 6;          /* total after shifting each die by -1 */
    for (idx_t k = 0; k <= 6; ++k) {
        idx_t sub = k * 6;
        if (sub > s) break;
        seq_t term = binom(6, k);                     /* C(6,k) */
        seq_t ways = binom(s - sub + 5, 5);           /* C(s-6k+5,5) */
        if (k & 1)
            count -= term * ways;
        else
            count += term * ways;
    }
    return count;
}
