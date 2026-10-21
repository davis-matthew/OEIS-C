/*
 * A088323  Number of numbers b>1 such that n is a repunit in base b
 *          representation.
 * https://oeis.org/A088323
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of integers b>1 such that n is a repunit in base b
 *            (i.e. n = 1 + b + b² + … + b^{k-1} for some k ≥ 2)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   repunit_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: For each possible length k (2 ≤ k ≤ floor(log₂(n+1)))
   binary search the base b; each k gives at most one b.  The term a(n)
   is increased by one for the trivial representation with k = 2
   (base b = n‑1). */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/repunit_value.h"

/* main sequence function */
seq_t a(idx_t n)
{
    /* a(2) = 0; a(3) = 1; for n≥3 the representation with k=2 exists. */
    if (n <= 2) return 0;

    seq_t count = 1;    /* contribution from base b = n-1 (k = 2) */

    /* Determine maximum possible length k:
       smallest repunit for base 2 is 2^k - 1.
       Find largest k such that 2^k - 1 ≤ n  ⇔  k ≤ floor(log₂(n+1)). */
    int k_max;
    if (n == UINT64_MAX) {
        k_max = 64;  /* special case: n+1 would overflow */
    } else {
        uint64_t t = n + 1;
        k_max = 0;
        while (t > 1) {
            t >>= 1;
            k_max++;
        }
    }

    /* Search for bases for all lengths k ≥ 3. */
    seq_t sn = (seq_t)n;   /* cast once for comparisons */
    for (int k = 3; k <= k_max; ++k)
    {
        idx_t low  = 2;
        idx_t high = n;        /* upper bound; repunit grows rapidly */
        bool found = false;

        while (low <= high)
        {
            idx_t mid = low + ((high - low) >> 1);
            bool exceeds;
            seq_t r = repunit_value(mid, k, sn, &exceeds);

            if (exceeds || r > sn) {
                high = mid - 1;
            } else if (r < sn) {
                low = mid + 1;
            } else { /* exact match */
                found = true;
                break;
            }
        }

        if (found) count++;
    }

    return count;
}
