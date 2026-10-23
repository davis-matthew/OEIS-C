/*
 * A020651  Denominators in recursive bijection from positive integers to
 *          positive rationals (the bijection is f(1) = 1, f(2n) = f(n)+1,
 *          f(2n+1) = 1/(f(n)+1)).
 * https://oeis.org/A020651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac, nice
 * Formula:   not stated
 * Range:     u8 n <= 5468, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Denominator of the recursive bijection to rationals
   defined by
        f(1) = 1
        f(2n)   = f(n) + 1
        f(2n+1) = 1 / (f(n) + 1)
   a(n) returns the denominator of f(n).  The computation follows
   the binary expansion of n and tracks the numerator and denominator
   of the current fraction.  Complexity: O(log n). */

#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    /* f(1) = 1/1 */
    if (n == 1) return (seq_t)1;

    seq_t p = 1;   /* numerator of the current fraction */
    seq_t q = 1;   /* denominator of the current fraction */

    /* highest set bit position (0‑based) */
    idx_t msb = floor_log2_uint64(n);

    /* process bits from the second most significant down to the least
       significant.  For each bit:
          0  → go to f(2k)   : (p + q) / q
          1  → go to f(2k+1) : q / (p + q)
    */
    for (idx_t i = msb; i > 0; --i) {
        if ((n >> (i - 1)) & 1) {          /* odd transition */
            seq_t newp = q;
            seq_t newq = p + q;
            p = newp;
            q = newq;
        } else {                           /* even transition */
            p = p + q;                     /* q stays the same */
        }
    }

    return q;   /* denominator of f(n) */
}
