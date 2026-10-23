/*
 * A117192  Number of partitions of n into distinct parts with an even rank.
 * https://oeis.org/A117192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   number of partitions of n into distinct parts with even rank
 * Range:     u8 n <= 33, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   free_seq_array, seq_alloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/seq_alloc_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* n >= 1 (offset 1).  For safety, if n==0 return 0. */
    if (n == 0)
        return 0;

    /* Two parity arrays for partitions using parts up to the current limit.
       dp0[s] – partitions of sum s with an even number of parts,
       dp1[s] – partitions of sum s with an odd number of parts.      */
    static seq_t *dp0 = NULL;
    static seq_t *dp1 = NULL;
    static size_t max_alloc = 0;   /* current allocation size (>= n+1)  */
    static int inited = 0;         /* 0 = not yet allocated, 1 = allocated */

    /* Reallocate if needed.  Grab new arrays, zero‑init them, and free old ones. */
    if (max_alloc <= n) {
        size_t new_size = (size_t)n + 1;
        seq_t *new0 = seq_alloc_array(new_size);
        seq_t *new1 = seq_alloc_array(new_size);
        for (size_t i = 0; i < new_size; ++i) {
            new0[i] = 0;
            new1[i] = 0;
        }
        if (inited) {
            free_seq_array(dp0);
            free_seq_array(dp1);
        }
        dp0 = new0;
        dp1 = new1;
        max_alloc = new_size;
        inited   = 1;
    } else {
        /* Zero‑initialize indices that will be overwritten in this call. */
        for (size_t i = 0; i <= (size_t)n; ++i) {
            dp0[i] = 0;
            dp1[i] = 0;
        }
    }

    /* Base: empty partition has sum 0 and even part count. */
    dp0[0] = 1;
    dp1[0] = 0;

    seq_t result = 0;

    for (idx_t L = 1; L <= n; ++L) {
        /* Parity of the new part: we need L and M (total parts) to have same parity.
           Let M = P + 1 (where P is parity of parts before adding L).  We need:
               L % 2 == (P + 1) % 2
           So required P parity = 1 - (L % 2). */
        size_t needP = 1 - (L & 1U);

        idx_t rem = n - L;          /* remaining sum after taking part L */
        if (rem <= n) {
            result += (needP == 0) ? dp0[rem] : dp1[rem];
        }

        /* Update dp arrays to include part L.  Iterate sums in decreasing order
           so that each new value depends only on old data.  Adding L toggles
           the parity of the part count. */
        for (idx_t s = n; s >= L; --s) {
            seq_t from0 = dp0[s - L];
            seq_t from1 = dp1[s - L];
            dp0[s] += from1;   /* even count = old even + old odd (from1) */
            dp1[s] += from0;   /* odd  count = old odd  + old even (from0) */
        }
    }

    return result;
}
