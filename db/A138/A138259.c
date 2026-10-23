/*
 * A138259  Smallest positive integer m such that n divides [6^m/m]
 *          (=A129796(m)).
 * https://oeis.org/A138259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest positive integer m such that * n divides ⌊6^m /
 *            m⌋. (OEIS A138259)
 * Range:     u8 n <= 16, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mod_pow.h"

/* Compute the sequence a(n).  The function is defined for n >= 1. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n = 0 */

    /* Search m ≥ 1.  We use the congruence derived in the analysis:
     * floor(6^m / m) mod n == 0  ⇔  (6^m mod (n*m)) / m == 0.
     *
     * 6^m mod (n*m) is computed with mod_pow.  The quotient is < n,
     * so its comparison to zero is equivalent to checking divisibility. */
    for (idx_t m = 1; ; ++m) {
        seq_t modulus = (seq_t)n * (seq_t)m;          /* n * m   */
        seq_t r       = mod_pow((seq_t)6, m, modulus); /* 6^m mod modulus */
        if (r / m == 0) {
            return (seq_t)m;                          /* minimal m found */
        }
    }
}
