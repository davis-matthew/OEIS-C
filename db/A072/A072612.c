/*
 * A072612  Minimal value of { abs(n-sigma(k)) : k>0 }.
 * https://oeis.org/A072612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = min_{k >= 1} | n - sigma(k) | (sigma(k) = sum of divisors
 *            of k)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *sigma_cache = NULL; /* sigma[1..N] */
static idx_t sigma_len   = 0;      /* current size of sigma_cache (max precomputed n) */

/* Recompute sigma[1..max_n] from scratch */
static void recompute_sigma(idx_t max_n)
{
    /* allocate new array of size max_n + 1 */
    seq_t *new_sigma = (seq_t*)malloc((max_n + 1) * sizeof(seq_t));
    if (!new_sigma) return; /* allocation failed; leave cache unchanged */

    /* initialize to zero */
    for (idx_t i = 0; i <= max_n; ++i) new_sigma[i] = (seq_t)0;

    /* sieve-like computation of sigma */
    for (idx_t d = 1; d <= max_n; ++d)
    {
        for (idx_t j = d; j <= max_n; j += d)
            new_sigma[j] += (seq_t)d;
    }

    /* free old cache and replace */
    free(sigma_cache);
    sigma_cache = new_sigma;
    sigma_len   = max_n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined but safe */

    /* ensure sigma_cache covers [1..n] */
    if (n > sigma_len)
        recompute_sigma(n);

    /* compute minimal distance */
    seq_t n_seq = (seq_t)n;
    seq_t min_diff = n_seq; /* maximum possible difference */

    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t sigma_k = sigma_cache[k];
        seq_t diff = (n_seq >= sigma_k) ? (n_seq - sigma_k) : (sigma_k - n_seq);
        if (diff < min_diff) min_diff = diff;
        if (min_diff == (seq_t)0) break; /* earliest possible */
    }
    return min_diff;
}
