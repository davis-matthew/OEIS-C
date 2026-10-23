/*
 * A126213  a(n) = sum of the d(k)'s, where 1<=k<=n and d(k) is equal to any
 *          divisor of n, where d(k) is the number of positive divisors of k.
 * https://oeis.org/A126213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = ∑_{k=1}^{n} τ(k) , where τ(k) | n and τ(k) is the number
 *            of positive divisors of k
 * Range:     u8 n <= 71, u16 n <= 7919, u32 n <= 9772, u64 n <= 9772,
 *            u128 n <= 9772, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for divisor counts; indices 0..limit */
static seq_t *tau_cache = NULL;
static idx_t   tau_limit = 0;

/* ensure that tau_cache contains τ(i) for all 0 <= i <= limit */
static void ensure_tau_limit(idx_t limit)
{
    if (limit <= tau_limit)
        return;                     /* already adequate */

    /* Allocate a new table of size limit+1, free the old one */
    seq_t *new_cache = (seq_t *)malloc((limit + 1) * sizeof(seq_t));
    if (!new_cache)
        abort();                     /* memory allocation failed */

    if (tau_cache)
        free(tau_cache);

    /* Initialize all entries to zero */
    for (idx_t i = 0; i <= limit; ++i)
        new_cache[i] = 0;

    /* Standard O(n log n) divisor sieve */
    for (idx_t i = 1; i <= limit; ++i) {
        for (idx_t j = i; j <= limit; j += i) {
            new_cache[j] += (seq_t)1;
        }
    }

    tau_cache = new_cache;
    tau_limit = limit;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* offset starts at 1 */

    ensure_tau_limit(n);

    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t tk = tau_cache[k];
        if ( ((seq_t)n) % tk == 0 )
            sum += tk;
    }
    return sum;
}
