/*
 * A169599  Numbers that are congruent to {4, 23} mod 29.
 * https://oeis.org/A169599
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2k) = 29k-6, a(2k+1) = 29k+4 (k ≥ 0), offset 1
 * Range:     u8 n <= 18, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n & 1U) {                    /* odd n = 2k+1 */
        idx_t k = (n - 1) / 2;
        return (seq_t)29 * (seq_t)k + (seq_t)4;
    } else {                         /* even n = 2k */
        idx_t k = n / 2;
        return (seq_t)29 * (seq_t)k - (seq_t)6;
    }
}
