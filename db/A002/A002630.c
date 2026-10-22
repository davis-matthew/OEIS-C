/*
 * A002630  Number of permutations of length n with two 3-sequences.
 * https://oeis.org/A002630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{m} S(m) * ( m*C(m-1, n-3-m) + C(m,2)*C(m-2, n-4-m)
 *            ), where S(m) = A000255(m-1) counts permutations of m blocks
 *            with no successions, A000255: T(0) = 1, T(1) = 1, T(k) =
 *            k*T(k-1) + (k-1)*T(k-2). (Blocks of consecutive ascending
 *            integers; two 3-sequences arise from one block of length 4...
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 14, u64 n <= 22, u128 n <= 24,
 *            big any n
 * Helpers:   a2630_binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/a2630_binom.h"

seq_t a(idx_t n) {
    if (n < 4) return 0;
    seq_t Tp = 0, Tc = 1; /* Tc = T(m-1) = S(m) */
    seq_t total = 0;
    seq_t cA = 0, cB = 0;
    bool inA = false, inB = false;
    for (idx_t m = 1; m + 3 <= n; m++) {
        /* case A: one block of length 4 */
        {
            idx_t b = n - 3 - m, aa = m - 1;
            if (b <= aa) {
                if (!inA) { cA = a2630_binom(aa, b); inA = true; }
                total += (seq_t)m * cA * Tc;
                if (b >= 1) {
                    cA = cA * (seq_t)(aa + 1) * (seq_t)b
                         / ((seq_t)(aa - b + 1) * (seq_t)(aa - b + 2));
                }
            }
        }
        /* case B: two blocks of length 3 */
        if (m >= 2 && m + 4 <= n) {
            idx_t b = n - 4 - m, aa = m - 2;
            if (b <= aa) {
                if (!inB) { cB = a2630_binom(aa, b); inB = true; }
                seq_t coef = (seq_t)m * (seq_t)(m - 1) / 2;
                total += coef * cB * Tc;
                if (b >= 1) {
                    cB = cB * (seq_t)(aa + 1) * (seq_t)b
                         / ((seq_t)(aa - b + 1) * (seq_t)(aa - b + 2));
                }
            }
        }
        seq_t Tn = (seq_t)m * Tc + (seq_t)(m - 1) * Tp;
        Tp = Tc;
        Tc = Tn;
    }
    return total;
}
