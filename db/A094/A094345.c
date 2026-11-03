/*
 * A094345  Sum of all digits in ternary expansions of 0, ..., n.
 * https://oeis.org/A094345
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 8285, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 8865 (2026-11-01)
 * Helpers:   none
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

static seq_t ternary_sum_upto(idx_t n)
{
    /* Base case: compute directly for n < 3. */
    if (n < 3) {
        seq_t total = 0;
        for (idx_t i = 0; i <= n; ++i) {
            idx_t t = i;
            seq_t s = 0;
            while (t > 0) {
                s += t % 3;
                t /= 3;
            }
            total += s;
        }
        return total;
    }

    /* Find largest power of 3 ≤ n. */
    idx_t m = 1;
    while (m * 3 <= n) {
        m *= 3;
    }

    idx_t d = n / m;       /* most significant ternary digit (1 or 2) */
    idx_t r = n % m;       /* remaining part */

    /* Recurrence components.  Cast to seq_t for arithmetic. */
    seq_t part1 = (seq_t)d * ternary_sum_upto(m - 1);
    seq_t part2 = (seq_t)m * (seq_t)d * (seq_t)(d - 1) / 2;
    seq_t part3 = (seq_t)d * (seq_t)(r + 1);
    seq_t part4 = ternary_sum_upto(r);

    return part1 + part2 + part3 + part4;
}

seq_t a(idx_t n)
{
    return ternary_sum_upto(n);
}
