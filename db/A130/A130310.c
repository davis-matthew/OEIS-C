/*
 * A130310  Minimal (or "greedy") Lucas representation of n, in which L(0) = 2
 *          and L(2) = 3 are not allowed in the same representation (hence the
 *          correct representation of the integer 5 is 1010 rather than 101).
 *          A binary system of integers with Lucas numbers (A000032) as a
 *          base.
 * https://oeis.org/A130310
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the minimal binary string of Lucas representation of n
 *            using Lucas numbers L0 = 2, L1 = 1, Lk = Lk‑1 + Lk‑2 for k ≥ 2,
 *            with the additional rule that L0 and L2 may not both be chosen
 *            in a representation. The function returns the decimal integer
 *            whose digits are that binary string
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 122, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* This implementation works for any seq_t and idx_t supplied by the
   build.  It builds all Lucas numbers up to n, computes which sums of
   the smaller Lucas numbers are attainable (dynamic programming), and
   then reconstructs the lexicographically smallest valid representation
   by scanning from the largest Lucas number downwards.  The algorithm
   runs in O(m · n) time and O(m · n) memory, which is well below the
   limits for n ≤ 10000. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Maximal number of Lucas numbers needed is tiny (≈ log_φ(n)). */
    seq_t luc[64];
    unsigned int m = 1;
    luc[0] = 2;          /* L0 */
    luc[1] = 1;          /* L1 */
    while (luc[m] <= (seq_t)n) {
        ++m;
        luc[m] = luc[m-1] + luc[m-2];
    }
    unsigned int maxIdx = m-1;          /* highest index with Lucas ≤ n */

    /* DP table: dp[i][s] is 1 if sum s can be obtained using Lucas
       numbers with indices < i.  We allocate one row per i. */
    unsigned int sz = (unsigned int)n + 1;
    int *dp[65];
    for (unsigned int i = 0; i <= maxIdx; ++i) {
        dp[i] = (int*)calloc(sz, sizeof(int));
    }
    dp[0][0] = 1;     /* only sum 0 is possible with no numbers */

    for (unsigned int i = 0; i < maxIdx; ++i) {
        for (unsigned int s = 0; s <= n; ++s) {
            if (!dp[i][s]) continue;
            dp[i+1][s] = 1;                       /* don't use luc[i] */
            unsigned int ns = s + (unsigned int)luc[i];
            if (ns <= n) dp[i+1][ns] = 1;          /* use luc[i] */
        }
    }

    seq_t result = 0;
    seq_t rem = (seq_t)n;
    bool usedL2 = false;          /* true if L2 (value 3) was chosen */

    /* Reconstruct the representation from highest index to 0. */
    for (int i = maxIdx; i >= 0; --i) {
        if (luc[i] <= rem) {
            /* We may set bit i if the remaining sum can be formed with
               indices lower than i and (if i == 0) L2 was not used. */
            bool ok = !(i == 0 && usedL2) &&
                      (dp[i][(unsigned int)(rem - luc[i])]);
            if (ok) {
                result = result * 10 + 1;
                rem -= luc[i];
                if (i == 2) usedL2 = true;
                continue;
            }
        }
        result = result * 10;   /* bit i is 0 */
    }

    /* Free DP rows. */
    for (unsigned int i = 0; i <= maxIdx; ++i) free(dp[i]);

    return result;
}
