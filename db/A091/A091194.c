/*
 * A091194  Number of abundant numbers <= n.
 * https://oeis.org/A091194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of abundant numbers ≤ n (σ(n) > 2n)
 * Range:     u8 n <= 420, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    idx_t i, j;
    /* allocate array for σ(0..n), initialised to 0 */
    seq_t *sigma = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    if (!sigma) return 0;  /* unlikely in tests */

    /* compute sum of divisors: σ(k) = Σ_{d|k} d */
    for (i = 1; i <= n; ++i) {
        for (j = i; j <= n; j += i) {
            sigma[j] += (seq_t)i;
        }
    }

    seq_t count = 0;
    for (i = 1; i <= n; ++i) {
        /* abundant if proper divisor sum > k, i.e. σ(k) > 2k */
        if (sigma[i] > ((seq_t)i * 2)) ++count;
    }

    free(sigma);
    return count;
}
