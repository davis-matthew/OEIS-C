/*
 * A132952  a(n) is the number of isolated totatives of n.
 * https://oeis.org/A132952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of isolated totatives of n
 * Range:     u8 n <= 331, u16 n <= 9337, u32 n <= 9337, u64 n <= 9337,
 *            u128 n <= 9337, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes by checking each totative k (1 ≤ k < n) and verifying that both
   neighbors k−1 and k+1 are not coprime to n. */
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n <= 1) return 0;

    seq_t isolated = 0;
    for (idx_t k = 1; k < n; ++k) {
        /* Is k a totative of n? */
        if (gcd((seq_t)k, (seq_t)n) != 1)
            continue;

        /* Check neighbors */
        if (gcd((seq_t)(k - 1), (seq_t)n) == 1)
            continue;               /* (k-1) coprime → not isolated */
        if (gcd((seq_t)(k + 1), (seq_t)n) == 1)
            continue;               /* (k+1) coprime → not isolated */

        /* Both neighbors not coprime → isolated */
        ++isolated;
    }
    return isolated;
}
