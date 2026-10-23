/*
 * A157806  Absolute value of the difference between numerator and denominator
 *          of fractions arranged by Cantor's ordering (1/1, 2/1, 1/2, 1/3,
 *          3/1, 4/1, 3/2, 2/3, 1/4, 1/5, 5/1, 6/1, ...) with equivalent
 *          fractions removed.
 * https://oeis.org/A157806
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS A157806:
   a(n) = |p - q| where p/q is the nth reduced positive rational in Cantor enumeration
   (generated in antidiagonal zig‑zag order with duplicates removed). */
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    idx_t pos = 0;                 /* current term index (0‑based)   */
    for (idx_t k = 2; ; ++k) {     /* k = numerator+denominator   */
        if ((k & 1) == 0) {        /* even: ascending i = 1..k-1 */
            for (idx_t i = 1; i < k; ++i) {
                idx_t j = k - i;
                if (gcd((seq_t)i, (seq_t)j) == 1) {
                    if (pos == n) {
                        seq_t diff = (i > j) ? (seq_t)(i - j) : (seq_t)(j - i);
                        return diff;
                    }
                    ++pos;
                }
            }
        } else {                   /* odd: descending i = k-1 .. 1 */
            for (idx_t i = k - 1; i >= 1; --i) {
                idx_t j = k - i;
                if (gcd((seq_t)i, (seq_t)j) == 1) {
                    if (pos == n) {
                        seq_t diff = (i > j) ? (seq_t)(i - j) : (seq_t)(j - i);
                        return diff;
                    }
                    ++pos;
                }
            }
        }
    }
}
