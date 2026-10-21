/*
 * A213991  Number of 2 X 2 0..n matrices whose square is also a 0..n matrix.
 * https://oeis.org/A213991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 2×2 0..n matrices M such that M² is also a 0..n
 *            matrix Computation: iterate over possible a,d (both ≤√n), and
 *            for each compute the number of admissible b,c pairs by counting
 *            pairs (b,c) with 0≤b,c≤⌊n/(a+d)⌋ and bc≤min(n−a²,n−d²). O(n log
 *            n) time, O(1) memory
 * Range:     u8 n <= 9, u16 n <= 215, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_bc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/count_bc.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t result = 0;
    unsigned long long Nmax = (unsigned long long)n;

    /* compute m = floor(sqrt(n)) */
    unsigned long long m = 0;
    while ((m + 1) * (m + 1) <= Nmax) ++m;

    for (unsigned long long a = 0; a <= m; ++a) {
        unsigned long long a_sq = a * a;
        for (unsigned long long d = 0; d <= m; ++d) {
            unsigned long long d_sq = d * d;
            if (a_sq > Nmax || d_sq > Nmax) continue;   /* T < 0, impossible */

            unsigned long long T = Nmax - a_sq;
            if (Nmax - d_sq < T) T = Nmax - d_sq;

            unsigned long long s = a + d;
            if (s == 0) {  /* a = d = 0 */
                result += count_bc(Nmax, Nmax);
                continue;
            }

            unsigned long long N = Nmax / s;   /* floor(n/(a+d)) */
            result += count_bc(N, T);
        }
    }
    return result;
}
