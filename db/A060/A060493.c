/*
 * A060493  A diagonal of A036969.
 * https://oeis.org/A060493
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n + 1)*(n + 2)*(2*n + 1)*(2*n + 3)*(5*n - 1)/360
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 64, u64 n <= 2630,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* the six factors of the numerator in seq_t to avoid overflow */
    seq_t factors[6];
    factors[0] = (seq_t)n;
    factors[1] = (seq_t)n + 1;
    factors[2] = (seq_t)n + 2;
    factors[3] = ((seq_t)2 * n) + 1;
    factors[4] = ((seq_t)2 * n) + 3;
    factors[5] = ((seq_t)5 * n) - 1;

    /* primes contained in 360 = 2^3 * 3^2 * 5 */
    int need2 = 3, need3 = 2, need5 = 1;
    seq_t res = 1;

    for (int i = 0; i < 6; ++i) {
        seq_t f = factors[i];
        while (need2 > 0 && (f % 2) == 0) {
            f /= 2;
            need2--;
        }
        while (need3 > 0 && (f % 3) == 0) {
            f /= 3;
            need3--;
        }
        while (need5 > 0 && (f % 5) == 0) {
            f /= 5;
            need5--;
        }
        res *= f;
    }
    return res;
}
