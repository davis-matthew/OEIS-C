/*
 * A076537  Map positive rational numbers to positive integers by diagonal
 *          method using c(p,q) = (p + q - 2) * (p + q - 1) / 2 + p where p
 *          and q are positive integers. a(n) is an increasing sequence
 *          including all c(p,q) where gcd(p,q) > 1.
 * https://oeis.org/A076537
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(n) is the nth smallest value of c(p,q) with gcd(p,q)>1,
   where c(p,q) = (p+q-2)*(p+q-1)/2 + p. */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t s = 2; ; ++s) {
        for (idx_t p = 1; p < s; ++p) {
            idx_t q = s - p;
            seq_t g = gcd((seq_t)p, (seq_t)q);
            if (g > (seq_t)1) {
                ++count;
                if (count == n) {
                    seq_t t = ((seq_t)(s - 2) * (seq_t)(s - 1)) / (seq_t)2;
                    return t + (seq_t)p;
                }
            }
        }
    }
}
