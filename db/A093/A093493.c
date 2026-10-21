/*
 * A093493  Define the total divisor symmetry of a number n to be the number
 *          of values r takes such that n-r and n+r have the same number of
 *          divisors. Sequence contains the total divisor symmetry of n.
 * https://oeis.org/A093493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{ r | 1 ≤ r ≤ n-1 , τ(n‑r) = τ(n+r) }|
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where τ(k) is the number of positive divisors of k. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Lazily compute divisor counts up to 2 * n. */
    static idx_t cur_max_n = 0;
    static seq_t *tau = NULL;

    /* Expand the sieve whenever a larger n is requested. */
    if (n > cur_max_n) {
        idx_t limit = 2 * n;               /* need τ(k) up to 2n-1 */
        seq_t *new_tau = (seq_t *)malloc((limit + 1) * sizeof(seq_t));
        if (!new_tau) return 0;            /* memory allocation failed */

        /* initialise and compute divisor counts */
        for (idx_t i = 0; i <= limit; ++i) new_tau[i] = 0;
        for (idx_t i = 1; i <= limit; ++i)
            for (idx_t j = i; j <= limit; j += i)
                new_tau[j] += 1;

        /* Replace old table if any. */
        if (tau) free(tau);
        tau = new_tau;
        cur_max_n = n;
    }

    seq_t count = 0;
    for (idx_t r = 1; r < n; ++r) {
        if (tau[n - r] == tau[n + r]) ++count;
    }
    return count;
}
