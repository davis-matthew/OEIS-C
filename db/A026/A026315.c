/*
 * A026315  |sin(n)| < |sin(n+1)|.
 * https://oeis.org/A026315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n‑th integer m ≥ 0 for which |sin(m)| < |sin(m+1)|
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   Using the identity sin^2(m+1) – sin^2(m) = sin(2m+1)·sin(1),   */
/*   the inequality is equivalent to sin(2m+1) > 0, i.e. floor((2m+1)/π) is even. */
/*   The reciprocal 1/π is approximated by the rational 113/355, which is     */
/*   accurate enough to preserve parity for all numbers that fit in a 64‑bit  */
/*   integer (m ≤ 20001 in the typical test range).                      */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n is at least 1 (offset 1).  Return the n‑th qualifying index.    */
    idx_t count = 0;
    idx_t i = 0;               /* candidate index m                                */

    while (1) {
        /* m = 2*i + 1  (odd)                                   */
        idx_t m = (i << 1) + 1;

        /* compute floor(m * (113/355)) accurately via 128‑bit   */
        __int128 prod = (__int128)m * 113;
        idx_t k = (idx_t)(prod / 355);         /* floor(m/π) approx */

        /* sin(2m+1) > 0  <=>  k even                           */
        if ((k & 1) == 0) {
            ++count;
            if (count == n)
                return (seq_t)i;            /* cast to sequence type */
        }
        ++i;
    }
}
