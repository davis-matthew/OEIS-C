/*
 * A076493  Number of common (distinct) decimal digits of n and n^2.
 * https://oeis.org/A076493
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = number of distinct common decimal digits of n and n^2
 * Range:     u8 n <= 15, u16 n <= 411, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t sq = nn * nn;
    unsigned mask_n = 0, mask_sq = 0;

    /* Process digits of n */
    if (nn == 0) {
        mask_n = 1;                     /* digit 0 */
    } else {
        while (nn > 0) {
            unsigned d = (unsigned)(nn % 10);
            mask_n |= 1u << d;
            nn /= 10;
        }
    }

    /* Process digits of n^2 */
    if (sq == 0) {
        mask_sq = 1;                     /* digit 0 */
    } else {
        while (sq > 0) {
            unsigned d = (unsigned)(sq % 10);
            mask_sq |= 1u << d;
            sq /= 10;
        }
    }

    unsigned common = mask_n & mask_sq;
    seq_t result = (seq_t)__builtin_popcount(common);
    return result;
}
