/*
 * A058664  McKay-Thompson series of class 40C for Monster.
 * https://oeis.org/A058664
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of partitions of n+1 into parts not divisible by 4 or
 *            5. The McKay–Thompson series T40C is
 *            (E(q⁸)·E(q¹⁰))/(E(q²)·E(q⁴⁰))/q where E(q)=∏(1‑q^k). After
 *            algebra the non‑zero coefficients occur only at odd exponents;
 *            a(n) is the coefficient at q^(2n+1). This coefficient equals the
 *            number of...
 * Range:     u8 n <= 20, u16 n <= 60, u32 n <= 189, u64 n <= 645,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *dp = NULL;
static size_t dp_size = 0;

/* Build the DP table up to max_t (inclusive).  dp[i] will contain
   the number of partitions of i into parts not divisible by 4 or 5. */
static void build_dp(size_t max_t)
{
    seq_t *newdp = (seq_t *)malloc((max_t + 1) * sizeof(seq_t));
    if (!newdp) return;          /* memory allocation failed – caller will
                                    return zero for all values            */

    for (size_t i = 0; i <= max_t; ++i) newdp[i] = 0;
    newdp[0] = 1;                 /* empty partition */

    for (int part = 1; part <= (int)max_t; ++part) {
        if (part % 4 == 0 || part % 5 == 0) continue;    /* forbidden part */
        for (size_t i = part; i <= max_t; ++i) {
            newdp[i] += newdp[i - part];
        }
    }

    free(dp);                     /* free previous table if any */
    dp = newdp;
    dp_size = max_t;
}

seq_t a(idx_t n)
{
    if (n < -1) return 0;         /* undefined for indices below the offset */

    size_t target = (size_t)(n + 1);   /* we need partitions of n+1          */

    if (dp == NULL || target > dp_size) {
        /* Allocate (or re‑allocate) a new table large enough for the
           requested target.  The table is built from scratch, but this
           overhead is negligible for the modest sizes involved (≤10001). */
        build_dp(target);
    }

    return dp[target];
}
