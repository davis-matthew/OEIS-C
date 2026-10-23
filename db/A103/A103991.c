/*
 * A103991  Reduced denominators of the hypercube line-picking integrand
 *          sqrt(Pi)*I(n,0).
 * https://oeis.org/A103991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(3k-2)=6k-3, a(3k-1)=6k-1, a(3k)=18k+3
 * Range:     u8 n <= 44, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    idx_t mod = n % 3;
    idx_t k;
    seq_t result;
    switch (mod) {
        case 1:                /* n = 3k-2 */
            k = (n + 2) / 3;
            result = 6 * (seq_t)k - 3;
            break;
        case 2:                /* n = 3k-1 */
            k = (n + 1) / 3;
            result = 6 * (seq_t)k - 1;
            break;
        case 0:                /* n = 3k   */
            k = n / 3;
            result = 18 * (seq_t)k + 3;
            break;
    }
    return result;
}
