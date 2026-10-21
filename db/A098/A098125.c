/*
 * A098125  Number of compositions of n where the largest part is less than
 *          the number of parts.
 * https://oeis.org/A098125
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{k=2..n} Σ_{j=0..⌊(n−k)/(k−1)⌋} (−1)^j * C(k, j) * C(n
 *            − j·(k−1) − 1, k−1) where C(a,b) is the binomial coefficient.
 *            The summands with a negative value of the upper argument are
 *            skipped (they are 0). The term with k=1 is omitted because no
 *            composition of a positive integer has largest pa...
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 31, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

# include <stdlib.h>
# include <stddef.h>
# include "../utils/binom.h"

static seq_t *seq_cache = NULL;      /* memoised a(n) values, indices 0..max_n */
static idx_t cache_size = 0;         /* currently computed up to cache_size */

/* compute values a(1..n) and extend the cache if necessary */
static void compute_up_to(idx_t n)
{
    if (n <= cache_size) return;

    /* extend the cache */
    seq_cache = (seq_t *)realloc(seq_cache, (n + 1) * sizeof(seq_t));
    if (!seq_cache) exit(1);  /* out of memory */

    /* initialise new entries to zero */
    for (idx_t i = cache_size + 1; i <= n; ++i) seq_cache[i] = 0;

    /* compute successive terms */
    for (idx_t v = cache_size + 1; v <= n; ++v)
    {
        seq_t pos = 0;   /* sum of terms with even j (sign +) */
        seq_t neg = 0;   /* sum of terms with odd  j (sign −) */

        /* iterate over possible number of parts k */
        for (idx_t k = 2; k <= v; ++k)
        {
            /* condition: n must not exceed k(k−1), otherwise no composition */
            if (v > k * (k - 1)) continue;

            for (idx_t j = 0; ; ++j)
            {
                idx_t N = v - j * (k - 1) - 1;   /* upper argument of the second binomial */
                if (N < k - 1) break;            /* C(N, k−1) would be zero */

                seq_t term = binom(k, j);
                term = term * binom(N, k - 1);

                if ((j & 1) == 0) pos += term; else neg += term;
            }
        }

        seq_cache[v] = pos - neg;   /* pos ≥ neg because result is non‑negative */
    }

    cache_size = n;
}

/* external linkage required by the framework */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* outside the offset, defined as 0 */
    compute_up_to(n);
    return seq_cache[n];
}
