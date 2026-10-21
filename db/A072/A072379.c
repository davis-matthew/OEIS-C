/*
 * A072379  Sum_{k<=n} (sigma(k)^2), where sigma(k) denotes the sum of the
 *          divisors of k A000203.
 * https://oeis.org/A072379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..n} σ(k)^2 where σ(k) is the sum of the divisors
 *            of k
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1624, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *sigma = NULL;   /* σ(1..N) */
static seq_t *pref  = NULL;   /* prefix sums Σ σ(i)^2 */
static idx_t   cached = 0;    /* maximum n that has been precomputed */

seq_t a(idx_t n) {
    if (n == 0) return 0;                     /* undefined but safe */

    if (n > cached) {
        /* free old buffers if any */
        free(sigma);
        free(pref);

        /* allocate new buffers (1-based indexing, +1 for dummy 0) */
        sigma = (seq_t*)calloc((size_t)(n + 1), sizeof(seq_t));
        pref  = (seq_t*)calloc((size_t)(n + 1), sizeof(seq_t));
        if (!sigma || !pref) {
            /* allocation failure – should never happen in tests */
            return 0;
        }

        /* compute divisor sums σ(k) using a simple sieve */
        for (idx_t i = 1; i <= n; ++i) {
            seq_t add = (seq_t)i;          /* cast i to the proper integer type */
            for (idx_t j = i; j <= n; j += i) {
                sigma[j] += add;
            }
        }

        /* build prefix sums of σ(k)^2 */
        pref[0] = 0;
        for (idx_t i = 1; i <= n; ++i) {
            seq_t sq = sigma[i] * sigma[i];  /* all calculations stay in seq_t */
            pref[i] = pref[i-1] + sq;
        }

        cached = n;
    }

    return pref[n];
}
