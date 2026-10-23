/*
 * A182187  a(n) is the least m >= n such that the Hamming distance D(n,m) =
 *          2.
 * https://oeis.org/A182187
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = least m >= n with Hamming distance(n, m) = 2.
 *            Implementation uses the OEIS formula: if n mod 4 == 0 : a(n) = n
 *            + 3 if n mod 4 == 1 : a(n) = n + 2^(A007814(n+1)-1) if n mod 4
 *            == 3 : a(n) = n + 2^(A007814(n+1)-1) if n mod 4 == 2 : a(n) = n
 *            + 2^(A007814(n+2)-1) where A007814(k) is the exponen...
 * Range:     u8 n <= 253, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* Handle the divisible-by-4 case first, since it is trivial. */
    if ((n & 3ULL) == 0ULL) {
        return (seq_t)n + (seq_t)3;
    }

    /* For the other cases we need the number of trailing zeros in a small
       integer (n+1 or n+2).  The value fits into uint64_t because n is an
       index of type idx_t (which is a 64‑bit unsigned integer).  We compute
       the trailing zero count manually to keep the code portable. */
    uint64_t v;
    unsigned tz = 0;          /* trailing zero count */

    if ((n & 1ULL) == 1ULL) {  /* n is odd  (1 or 3 mod 4) */
        v = n + 1ULL;         /* n+1 is even, at least 2 */
    } else {                  /* n is 2 mod 4 */
        v = n + 2ULL;         /* n+2 is even, at least 4 */
    }

    /* Count trailing zeros of v.  Since v >= 2, the loop executes at least once. */
    while ((v & 1ULL) == 0ULL) {
        tz++;
        v >>= 1;
    }

    /* tz is at least 1.  We need 2^(tz-1).  Use pow2 utility to be safe for
       arbitrary-precision seq_t types. */
    seq_t add = pow2((idx_t)(tz - 1));
    return (seq_t)n + add;
}
