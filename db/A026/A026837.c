/*
 * A026837  Number of partitions of n into distinct parts, the greatest being
 *          odd.
 * https://oeis.org/A026837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A026837: Number of partitions of n into distinct parts with greatest part odd.
   Computation via dynamic programming derived from the generating function
   G(x) = Σ_{k≥1} x^{2k‑1} ∏_{j=1}^{2k-2} (1 + x^j).  */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    size_t N = (size_t)n;
    /* count[i]  – total partitions of i into distinct parts (any largest part).
       dpodd[i] – partitions of i with largest part odd.           */
    seq_t *count = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    seq_t *dpodd = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    if (!count || !dpodd) return 0;   /* insufficient memory → 0 */

    for (size_t i = 0; i <= N; ++i) {
        count[i] = dpodd[i] = 0;
    }
    count[0] = 1;   /* empty partition used as base for single‑part counts */

    for (uint64_t part = 1; part <= n; ++part)
    {
        size_t p = (size_t)part;
        /* iterate sums in descending order to ensure each part is used at most once */
        for (size_t s = N; s >= p; --s)
        {
            if (part & 1ULL)            /* part is odd → contributes to dpodd */
                dpodd[s] += count[s - p];
            count[s] += count[s - p];
            if (s == 0) break;         /* prevent wrap‑around when s==p==0 (never happens) */
        }
    }

    seq_t result = dpodd[N];
    free(count);
    free(dpodd);
    return result;
}
