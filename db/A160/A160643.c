/*
 * A160643  Bisect A053445 then calculate the first differences of the
 *          resulting sequence.
 * https://oeis.org/A160643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of partitions of the odd integer 2n+1 into parts ≥
 *            2 such that the largest part occurs at least three times
 * Range:     u8 n <= 19, u16 n <= 40, u32 n <= 100, u64 n <= 281,
 *            u128 n <= 887, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* the type seq_t/idx_t is supplied by the build system */
static seq_t *seqA = NULL;          /* array of a(n) values, 1‑based */
static size_t computed_N = 0;       /* highest n for which seqA is valid */

/* helper to recompute seqA up to a given N (inclusive) */
static void compute_up_to(idx_t N)
{
    if (N <= computed_N)
        return;                       /* already computed up to N */

    /* free previous array */
    free(seqA);
    seqA = NULL;

    /* limits */
    idx_t maxM = 2 * N + 1;           /* largest odd number that can appear */
    idx_t Lmax = maxM / 3;            /* largest part that may appear at least 3 times */

    /* dynamic array dp[s] = number of partitions of s with parts ≤ current L */
    size_t dpSize = (size_t)maxM + 1;
    seq_t *dp = (seq_t*)calloc(dpSize, sizeof(seq_t));
    if (!dp) return;                  /* allocation failed – unlikely in judge environment */
    dp[0] = (seq_t)1;                 /* one way to partition 0 */

    /* temporary array for the a(n) values */
    size_t aSize = (size_t)N + 1;
    seq_t *newA = (seq_t*)calloc(aSize, sizeof(seq_t));
    if (!newA) { free(dp); return; }

    /* Main loop over the largest part value L */
    for (idx_t L = 2; L <= Lmax; ++L) {
        /* Update dp to allow parts of size L (unbounded coin change) */
        for (idx_t s = L; s <= maxM; ++s) {
            dp[s] += dp[s - L];
        }

        /* Add contributions for odd numbers M = 2n+1 with 3L ≤ M */
        idx_t startM = 3 * L;
        if ((startM & 1) == 0)
            ++startM;                  /* make it odd */

        for (idx_t M = startM; M <= maxM; M += 2) {
            idx_t k = M - 3 * L;       /* remainder after taking 3 copies of L */
            idx_t nIdx = (M - 1) / 2;   /* the corresponding n index (1‑based) */

            if (nIdx <= N)
                newA[nIdx] += dp[k];
        }
    }

    /* store the computed array */
    seqA = newA;
    computed_N = (size_t)N;

    /* Free the temporary dp array */
    free(dp);
}

/* required external function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                /* offset is 1, but safe guard */

    compute_up_to(n);
    return seqA[(size_t)n];
}
