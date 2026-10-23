/*
 * A169784  Number of solutions to a^2 + b^2 + 5*c^2 = n.
 * https://oeis.org/A169784
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{c ≥ 0, 5c² ≤ n} (2 - δ_{c,0}) * r₂(n - 5c²) where
 *            r₂(m) = 4*(d₁(m) - d₃(m)) and d₁(m), d₃(m) are the numbers of
 *            divisors of m that are ≡ 1 (mod 4) and ≡ 3 (mod 4) respectively.
 *            The formula counts all integer solutions (a, b, c) to a² + b² +
 *            5c² = n
 * Range:     u8 n <= 504, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Allocate divisor-count arrays up to n */
    size_t N = (size_t)n;
    unsigned int *d1 = (unsigned int*)malloc((N + 1) * sizeof(unsigned int));
    unsigned int *d3 = (unsigned int*)malloc((N + 1) * sizeof(unsigned int));
    if (!d1 || !d3) /* graceful failure; actual build should handle this */
        return 0;

    /* Initialize arrays */
    for (size_t i = 0; i <= N; ++i) {
        d1[i] = 0;
        d3[i] = 0;
    }

    /* Count divisors congruent to 1 or 3 mod 4 */
    for (size_t d = 1; d <= N; ++d) {
        if (d % 4 == 1) {
            for (size_t m = d; m <= N; m += d)
                ++d1[m];
        } else if (d % 4 == 3) {
            for (size_t m = d; m <= N; m += d)
                ++d3[m];
        }
    }

    /* Compute the sequence value */
    seq_t result = 0;
    for (idx_t c = 0; ; ++c) {
        idx_t sub = 5 * c * c;
        if (sub > n) break;
        idx_t m = n - sub;

        seq_t r2_val;
        if (m == 0) {
            r2_val = 1;          /* only (0,0) for a²+b²=0 */
        } else {
            r2_val = (seq_t)4 * ((seq_t)d1[m] - (seq_t)d3[m]);
        }

        seq_t weight = (c == 0) ? 1 : 2;  /* c and -c give distinct solutions when c>0 */
        result += weight * r2_val;
    }

    free(d1);
    free(d3);
    return result;
}
