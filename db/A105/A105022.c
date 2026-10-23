/*
 * A105022  Entries in the n-th row of Pascal's triangle that have the 2's bit
 *          set in their binary expansion.
 * https://oeis.org/A105022
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of entries in row n of Pascal's triangle whose
 *            binomial coefficient has the second least‑significant bit set
 *            (i.e., C(n,k) % 4 is 2 or 3)
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t count   = 0;          /* result counter */
    unsigned pc_n = __builtin_popcountll((unsigned long long)n);

    seq_t num_prod = 1;         /* product of odd parts of numerator terms modulo 4 */
    seq_t den_prod = 1;         /* product of odd parts of denominator terms modulo 4 */

    for (idx_t k = 0; k <= n; ++k) {
        unsigned pc_k  = __builtin_popcountll((unsigned long long)k);
        unsigned pc_nk = __builtin_popcountll((unsigned long long)(n - k));
        unsigned e2    = pc_k + pc_nk - pc_n;   /* exponent of 2 in C(n,k) */

        if (e2 == 1) {
            /* divisible by 2 but not by 4  → bit 1 set */
            count = count + 1;
        } else if (e2 == 0) {
            /* C(n,k) is odd: decide whether it is 3 (mod 4) */
            uint8_t den = (uint8_t)den_prod;          /* 1 or 3 */
            uint8_t num = (uint8_t)num_prod;          /* 1 or 3 */
            uint8_t inv_den = (den == 1) ? 1 : 3;     /* inverse of 1 or 3 modulo 4 */
            uint8_t mod4_val = (uint8_t)((num * inv_den) & 3);
            if (mod4_val == 3) {
                count = count + 1;
            }
        }

        /* Prepare the odd products for the next k+1 */
        if (k < n) {
            uint64_t t_num = n - k;          /* factor (n - k) in the numerator */
            while ((t_num & 1ULL) == 0) t_num >>= 1;
            num_prod = (num_prod * (t_num & 3ULL)) & 3;

            uint64_t t_den = k + 1;          /* factor (k + 1) in the denominator */
            while ((t_den & 1ULL) == 0) t_den >>= 1;
            den_prod = (den_prod * (t_den & 3ULL)) & 3;
        }
    }
    return count;
}
