/*
 * A097283  Contains exactly once every pair (i,j) satisfying 0 < i < j.
 * https://oeis.org/A097283
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the first component of the unordered pair (i,j) with
 *            0<i<j, listed lexicographically by the sum s=i+j and then by i.
 *            The sequence enumerates i, j alternately for each such pair
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   terms_up_to_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/terms_up_to_sum.h"

/* Main function required by the build */
seq_t a(idx_t n) {
    /* Handle the offset and trivial cases */
    if (n <= 1)
        return (seq_t)1;                     /* a(1)=1 */

    /* Find the smallest sum s with terms_up_to_sum(s) >= n using binary search */
    uint64_t low = 3, high = 4;
    while (terms_up_to_sum(high) < (uint64_t)n)
        high <<= 1;                           /* double until large enough */

    while (low <= high) {
        uint64_t mid = low + ((high - low) >> 1);
        if (terms_up_to_sum(mid) >= (uint64_t)n)
            high = mid - 1;
        else
            low = mid + 1;
    }
    uint64_t s = low;                        /* sum that contains the nth term */

    uint64_t prev_terms = terms_up_to_sum(s - 1);
    uint64_t offset     = (uint64_t)n - prev_terms - 1;  /* 0‑based within this sum */

    uint64_t pair_idx = offset >> 1;             /* index of the pair within this sum */
    uint64_t i_val    = pair_idx + 1;            /* first component of the pair */

    if ((offset & 1) == 0)                      /* even offset → i component */
        return (seq_t)i_val;
    else                                        /* odd offset  → j component */
        return (seq_t)(s - i_val);
}
