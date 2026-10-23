/*
 * A103924  Number of partitions of n into parts but with two kinds of parts
 *          of sizes 1,2,3,4 and 5.
 * https://oeis.org/A103924
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficient of x^n in * (∏_{k=1}^{5} (1 - x^k)^{-2}) *
 *            (∏_{j=6}^{∞} (1 - x^j)^{-1}) * – partitions of n into parts
 *            where sizes 1–5 may be chosen in two * colours each, while all
 *            larger sizes have a single colour
 * Range:     u8 n <= 8, u16 n <= 24, u32 n <= 78, u64 n <= 286,
 *            u128 n <= 1122, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

static seq_t *dp = NULL;    /* dp[i] = a(i) for 0 <= i <= precomp */
static idx_t  precomp = 0;   /* highest n that has been computed */

/* compute the sequence term */
seq_t a(idx_t n)
{
    /* extend the table if necessary */
    if (!dp || n > precomp) {
        /* allocate space for new values up to n */
        seq_t *newdp = alloc_seq_array((size_t)(n + 1));
        if (!newdp)               /* allocation failure – graceful */
            return (seq_t)0;

        /* initialise the array */
        newdp[0] = (seq_t)1;
        for (idx_t i = 1; i <= n; ++i)
            newdp[i] = (seq_t)0;

        /* coins of sizes 1–5, two colours each (i.e. two independent types) */
        for (int sz = 1; sz <= 5; ++sz) {
            for (int copy = 0; copy < 2; ++copy) {
                for (idx_t i = sz; i <= n; ++i) {
                    newdp[i] += newdp[i - sz];
                }
            }
        }

        /* coins of size ≥ 6, one colour each (unlimited supply) */
        for (idx_t sz = 6; sz <= n; ++sz) {
            for (idx_t i = sz; i <= n; ++i) {
                newdp[i] += newdp[i - sz];
            }
        }

        /* replace the old table */
        free_seq_array(dp);
        dp = newdp;
        precomp = n;
    }

    return dp[n];
}
