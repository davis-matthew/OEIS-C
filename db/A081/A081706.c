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
 * Validated: standard, pass, 77 terms through n = 9461 (2026-11-01)
 * Helpers:   none
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

/* Return true if x satisfies the condition for A081706 */
static inline bool _a081706_ok(seq_t x)
{
    /* odd numbers: must have even number of trailing 1’s */
    if ((x & 1) == 1) {
        idx_t count = 0;
        seq_t tmp = x;
        while ((tmp & 1) != 0) {
            ++count;
            tmp >>= 1;
        }
        return (count & 1) == 0;          /* even parity */
    }
    /* even numbers: trailing 0 followed by an odd number of 1’s in x>>1 */
    seq_t y = x >> 1;
    idx_t count = 0;
    while ((y & 1) != 0) {
        ++count;
        y >>= 1;
    }
    return (count & 1) == 1;              /* odd parity */
}

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
