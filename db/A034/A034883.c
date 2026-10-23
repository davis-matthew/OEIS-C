/*
 * A034883  Maximum length of Euclidean algorithm starting with n and any
 *          nonnegative i<n.
 * https://oeis.org/A034883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: maximum length of Euclidean algorithm starting 
   with (n,i) for all 0 ≤ i < n. */
#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t best = 0;
    for (idx_t i = 0; i < n; ++i) {
        idx_t a0 = n, b0 = i;
        seq_t steps = 0;
        while (b0 != 0) {
            idx_t r = a0 % b0;
            a0 = b0;
            b0 = r;
            ++steps;
        }
        if (steps > best) best = steps;
    }
    return best;
}
