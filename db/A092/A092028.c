/*
 * A092028  a(n) is the smallest m > 1 such that m divides n^m-1.
 * https://oeis.org/A092028
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 257, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = smallest m > 1 such that m divides n^m – 1.
   It is computed by testing successive m and checking
   whether n^m ≡ 1 (mod m). */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* Sequence is defined for n ≥ 3.  Values below the offset are
       undefined; return 0 for them. */
    if (n < 3) {
        return 0;
    }

    seq_t sn = (seq_t)n;            /* cast the index to seq_t once */

    /* Search for the smallest m > 1 that satisfies the condition. */
    for (idx_t m = 2; m < n; ++m) {
        /* Compute sn^m mod m using the helper function. */
        if (mod_pow(sn, m, (seq_t)m) == 1) {
            return (seq_t)m;
        }
    }

    /* In theory we should always find a solution before m == n.  As a
       safety net, return n (which always divides n^n – 1 by Fermat's
       little theorem when n is prime, and otherwise the loop would
       have found a smaller m). */
    return (seq_t)n;
}
