/*
 * A047186  Number of nonempty subsets of {1,2,...,n} in which exactly 3/4 of
 *          the elements are <= (n-2)/2.
 * https://oeis.org/A047186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1}^{⌊t/3⌋} C(t,3k) * C(n‑t,k) where t = ⌊(n‑2)/2⌋
 * Range:     u8 n <= 15, u16 n <= 25, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n) {
    if (n < 8) return 0;                     /* first non‑zero term at n = 8 */

    idx_t t     = (n / 2) - 1;               /* floor((n‑2)/2) */
    idx_t sizeB = n - t;                     /* |B| = n − t */

    idx_t kmax = t / 3;                      /* maximum k with 3k ≤ t */
    if (kmax == 0) return 0;

    seq_t sum = 0;
    for (idx_t k = 1; k <= kmax; ++k) {
        seq_t c1 = binom(t, 3 * k);          /* C(t,3k)   */
        seq_t c2 = binom(sizeB, k);          /* C(n‑t,k)  */
        sum += c1 * c2;                      /* add term   */
    }
    return sum;
}
