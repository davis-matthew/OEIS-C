/*
 * A074796  Number of numbers k <= n such that tau(k) == 2 (mod 3) where
 *          tau(k) = A000005(k) is the number of divisors of k.
 * https://oeis.org/A074796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{ k ≤ n : τ(k) ≡ 2 (mod 3) }| , τ(k) = number of
 *            divisors of k
 * Range:     u8 n <= 669, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *pref = NULL;     /* prefix counts, 1‑based */
static idx_t   cur_max = 0;    /* maximum n that has been precomputed */

/* Count of numbers k ≤ n with divisor count ≡ 2 (mod 3) */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* sequence is defined for n ≥ 1, but avoid UB */

    /* If we already have a prefix up to n, just return it */
    if (pref && n <= cur_max)
        return pref[n];

    /* Otherwise recompute the whole prefix up to the new maximum n */
    idx_t new_max = n;

    /* Allocate temporary array for divisor counts */
    seq_t *tau = (seq_t*)malloc((new_max + 1) * sizeof(seq_t));
    if (!tau)
        return 0;  /* Allocation failed – return 0 as a safe fallback */

    /* Initialize tau to zero */
    for (idx_t i = 0; i <= new_max; ++i)
        tau[i] = 0;

    /* Simple divisor sieve: for each d, add one to all multiples */
    for (idx_t d = 1; d <= new_max; ++d) {
        for (idx_t m = d; m <= new_max; m += d)
            ++tau[m];
    }

    /* Free old prefix storage */
    if (pref)
        free(pref);

    pref = (seq_t*)malloc((new_max + 1) * sizeof(seq_t));
    if (!pref)
        return 0;  /* Allocation failed */

    pref[0] = 0;   /* a(0) is unused but convenient for prefix sum */
    for (idx_t i = 1; i <= new_max; ++i) {
        pref[i] = pref[i - 1] +
                  ((tau[i] % 3ULL) == 2ULL ? 1 : 0);
    }

    free(tau);

    cur_max = new_max;
    return pref[n];
}
