/*
 * A170578  Number of reduced words of length n in Coxeter group on 41
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^47 = I.
 * https://oeis.org/A170578
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A170578: Number of reduced words of length n in the Coxeter group
   with generators S_i and relations S_i^2 = (S_i S_j)^47 = I.
   The sequence satisfies the linear recurrence
   a(0) = 1,
   a(1) = 41,
   a(2) = 1640,
   ...
   For n ≥ 47
   a(n) = 39·(a(n-1)+⋯+a(n-46)) – 780·a(n-47) + b(n),
   where b(n) = 1 if n = 0, 2 if 1 ≤ n ≤ 46, 0 otherwise.  This
   follows from the rational generating function
   (t^47 + 2t^46 + … + 2t + 1) / (1 – 39t – ⋯ – 39t^46 + 780t^47). */
#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;   /* cached values a(0)..a(k-1) */
static idx_t  cache_len = 0; /* how many entries are currently cached */

seq_t a(idx_t n)
{
    /* expand cache if necessary */
    if (cache_len <= n) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();        /* unrecoverable allocation failure */
        cache = tmp;
        /* compute new terms from the end of the previous cache up to n */
        for (idx_t i = cache_len; i <= n; ++i) {
            /* b(i): numerator coefficient */
            seq_t b = (i == 0) ? (seq_t)1 : ((i <= 46) ? (seq_t)2 : (seq_t)0);

            /* sum of the 46 most recent terms a(i-1)…a(i-46) */
            seq_t sum = 0;
            idx_t maxk = (i < 46) ? i : 46;
            for (idx_t k = 1; k <= maxk; ++k)
                sum += cache[i - k];

            /* compute a(i) = b + 39*sum - 780*a(i-47) (if i≥47) */
            seq_t v = b + (seq_t)39 * sum;
            if (i >= 47)
                v -= (seq_t)780 * cache[i - 47];
            cache[i] = v;
        }
        cache_len = new_len;
    }
    return cache[n];
}
