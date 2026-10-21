/*
 * A094345  Sum of all digits in ternary expansions of 0, ..., n.
 * https://oeis.org/A094345
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 8285, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ternary_sum_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A094345: Sum of all digits in ternary expansions of 0 … n.
   The function uses the standard digit‑DP recurrence
   (see e.g. Allouche & Shallit, 2003):

   Let S(n) be the required sum.
   For n < 3, S(n) = sum_{i=0}^n digit_sum_3(i).
   Otherwise let m = 3^⌊log₃ n⌋ (largest power of 3 ≤ n),
   d = n / m  (most significant ternary digit, 1 or 2),
   r = n % m  (remaining less‑significant part).
   Then
        S(n) = d · S(m‑1)                     // lower blocks
               + m · d·(d‑1)/2                // leading digits of the lower blocks
               + d · (r+1)                    // leading digit of the last partial block
               + S(r).                        // lower digits of the last partial block
*/

#include <stdint.h>   /* for idx_t and seq_t definitions */
#include "../utils/ternary_sum_upto.h"

seq_t a(idx_t n)
{
    return ternary_sum_upto(n);
}
