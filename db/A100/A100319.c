/*
 * A100319  Even numbers m such that at least one of m-1 and m+1 is composite.
 * https://oeis.org/A100319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A100319:
   Even numbers m such that at least one of m-1 and m+1 is composite.
   The sequence is the set of all even m except those where both m-1
   and m+1 are primes (i.e., the center of a twin‑prime pair).  */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Sequence offset is 1: a(1)=8.  n must be >=1. */
    if (n == 0) return 0;          /* undefined, but return 0 for safety */

    idx_t   count = 0;  /* how many valid even numbers found */
    seq_t   m     = 8;  /* first even number that satisfies the condition */

    /* Simple linear search.  The density of excluded numbers is very low,
       so O(n) time is adequate for n up to at least 10 000. */
    while (count < n)
    {
        bool p1 = is_prime((uint64_t)(m - 1));   /* check m-1 */
        bool p2 = is_prime((uint64_t)(m + 1));   /* check m+1 */

        if (!(p1 && p2)) {          /* at least one composite neighbor */
            count++;
            if (count == n) return m;   /* found the n-th term */
        }
        m += 2;   /* next even integer */
    }
    return m;   /* unreachable */
}
