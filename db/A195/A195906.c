/*
 * A195906  A periodic sequence.
 * https://oeis.org/A195906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 2000, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(1) = 22.  For n > 1, let
   a(n) = f(a(n-1)) where
     f(x) = 9*x          if x < 10
           = x/2         if x ≥ 10 and even
           = x-5         if x ≥ 10 and odd
   This generates a 5‑term periodic sequence. */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* outside the defined offset */
    seq_t val = (seq_t)22;
    for (idx_t i = 1; i < n; ++i) {
        if (val < (seq_t)10) {
            val = val * (seq_t)9;
        } else {
            if ((val & (seq_t)1) == 0) {
                val = val / (seq_t)2;
            } else {
                val = val - (seq_t)5;
            }
        }
    }
    return val;
}
