/*
 * A081706  Numbers n such that binary representation ends either in an odd
 *          number of ones followed by one zero or in an even number of ones.
 * https://oeis.org/A081706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _a081706_ok
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A081706: Numbers n such that binary representation ends either
   in an odd number of ones followed by one zero (for even n) or
   in an even number of ones (for odd n).  The function returns
   the n‑th such number (n>=1).  Computation is performed by
   scanning successive integers and selecting those that satisfy
   the trailing‑ones parity rule. */

#include <stdbool.h>
#include "../utils/_a081706_ok.h"

seq_t a(idx_t n)
{
    /* Find the n‑th qualifying integer, starting from 2. */
    idx_t found = 0;
    seq_t x = (seq_t)2;
    while (found < n) {
        if (_a081706_ok(x)) {
            ++found;
            if (found == n) return x;
        }
        ++x;
    }
    /* unreachable, but keep the compiler happy */
    return (seq_t)0;
}
