/*
 * A000127  Maximal number of regions obtained by joining n points around a
 *          circle by straight lines. Also number of regions in 4-space formed
 *          by n-1 hyperplanes.
 * https://oeis.org/A000127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = C(n-1,0) + C(n-1,1) + C(n-1,2) + C(n-1,3) + C(n-1,4)
 * Range:     u8 n <= 9, u16 n <= 36, u32 n <= 568, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   small_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/small_gcd.h"

seq_t a(idx_t n)
{
    seq_t m = (seq_t)(n - 1);
    seq_t c = 1;     /* C(m, k) */
    seq_t sum = 1;   /* C(m, 0) */
    unsigned long long k;
    for (k = 0; k < 4; k++) {
        if ((seq_t)k >= m) break;          /* remaining binomials are 0 */
        /* C(m, k+1) = C(m, k) * (m - k) / (k + 1), done without overflow */
        unsigned long long d = k + 1;
        unsigned long long g = small_gcd(c, d);
        seq_t f = m - (seq_t)k;
        c = (c / (seq_t)g) * (f / (seq_t)(d / g));
        sum += c;
    }
    return sum;
}
