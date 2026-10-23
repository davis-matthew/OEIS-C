/*
 * A046524  Number of coverings of Klein bottle with n lists.
 * https://oeis.org/A046524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = d(n) for odd n, a(n) = [3*d(n)+sigma(n/2)-d(n/2)]/2 for
 *            even n
 * Range:     u8 n <= 335, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   divisor_count_u64, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/divisor_count_u64.h"
#include "../utils/sigma_of.h"

seq_t a(idx_t n) {
    if (n & 1ULL) { /* odd n */
        return (seq_t)divisor_count_u64(n);
    } else { /* even n */
        const seq_t d_n  = (seq_t)divisor_count_u64(n);
        const idx_t  n2  = n >> 1;
        const seq_t d_n2 = (seq_t)divisor_count_u64(n2);
        const seq_t sigma_n2 = sigma_of(n2);
        const seq_t numerator = ((seq_t)3 * d_n) + sigma_n2 - d_n2;
        return numerator / 2;
    }
}
