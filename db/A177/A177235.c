/*
 * A177235  The number of non-divisors k of n, 1 < k < n, for which floor(n/k)
 *          is odd.
 * https://oeis.org/A177235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{i=1}^{⌊n/2⌋} d(n-i) - d(i) (OEIS A177235)
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* trivial cases */
    if (n <= 2) return 0;

    /* convert the index to a host integer type for array bounds */
    size_t N = (size_t)n;

    /* divisor count array, indices 0..N (0 unused) */
    uint32_t *d = (uint32_t *)malloc((N + 1) * sizeof(uint32_t));
    if (!d) exit(EXIT_FAILURE);

    /* initialise to zero */
    for (size_t i = 0; i <= N; ++i) d[i] = 0;

    /* classic divisor‑sieve; counts fit easily in 32 bits */
    for (size_t i = 1; i <= N; ++i) {
        for (size_t j = i; j <= N; j += i) {
            d[j]++;
        }
    }

    /* compute the two sums from the definition */
    idx_t m = n / 2;          /* ⌊n/2⌋ */
    seq_t sum_low = 0;        /* Σ_{i=1}^{m}   d(i)   */
    seq_t sum_high = 0;       /* Σ_{i=n-m}^{n-1} d(i)   */

    for (idx_t i = 1; i <= m; ++i) {
        sum_low += (seq_t)d[i];
    }
    for (idx_t i = n - m; i <= n - 1; ++i) {
        sum_high += (seq_t)d[i];
    }

    free(d);
    return sum_high - sum_low;
}
