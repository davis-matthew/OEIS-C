/*
 * A063985  Partial sums of cototient sequence A051953.
 * https://oeis.org/A063985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{x=1..n} (x - phi(x))
 * Range:     u8 n <= 35, u16 n <= 577, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a simple O(n log log n) spf sieve
   and computes the cototient sum directly.  It works for
   calls in any order and for any supported seq_t type. */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1, but guard against 0 */

    /* allocate SPF array (smallest prime factor for each integer) */
    uint64_t *spf = (uint64_t *)malloc((n + 1) * sizeof(uint64_t));
    if (!spf) return 0;     /* allocation failed */
    /* initialisation: spf[0] and spf[1] are left 0 */
    memset(spf + 1, 0, n * sizeof(uint64_t));

    /* classic sieve to fill spf */
    for (idx_t i = 2; i <= n; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            for (uint64_t j = (uint64_t)i * (uint64_t)i; j <= n; j += i) {
                if (spf[j] == 0) spf[j] = i;
            }
        }
    }

    /* compute the partial sums of the cototient sequence */
    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t phi = (seq_t)i;
        uint64_t x = i;
        uint64_t last_p = 0;
        while (x > 1) {
            uint64_t p = spf[x];
            if (p != last_p) {
                phi = phi / p * (seq_t)(p - 1);
                last_p = p;
            }
            x /= p;
        }
        sum += (seq_t)i - phi;
    }

    free(spf);
    return sum;
}
