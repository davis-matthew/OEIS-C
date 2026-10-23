/*
 * A048043  Number of nonempty subsets of {1,2,...,n} in which exactly 1/5 of
 *          the elements are <= (n+1)/3.
 * https://oeis.org/A048043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k≥1} C(L, k) · C(H, 4k) where L = ⌊(n+1)/3⌋, H = n –
 *            L, and the sum is over all k with k ≤ L, 4k ≤ H, and 5k ≤ n
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note that a(n)=0 for n<5 because the smallest non‑empty subset satisfying
   the condition has size 5. */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* n is at least 1.  For n<5, no subset satisfies the condition. */
    if (n < 5)
        return 0;

    /* Elements ≤ (n+1)/3 */
    idx_t L = (n + 1) / 3;          /* floor((n+1)/3) */
    idx_t H = n - L;               /* remaining elements */

    /* Maximum possible k */
    idx_t kmax = n / 5;            /* from 5k ≤ n */
    if (kmax > L)
        kmax = L;                  /* from k ≤ L */
    if (kmax > H / 4)
        kmax = H / 4;              /* from 4k ≤ H */

    seq_t total = 0;

    /* Sum over valid k */
    for (idx_t k = 1; k <= kmax; ++k) {
        seq_t left  = binomial(L, k);          /* C(L, k)   */
        seq_t right = binomial(H, 4 * k);      /* C(H, 4k)  */
        total += left * right;
    }

    return total;
}
