/*
 * A222733  Total sum of parts of multiplicity 5 in all partitions of n.
 * https://oeis.org/A222733
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k≥1} k * (p(n-5k) - p(n-6k)), where p(m) is the
 *            partition function (p(0)=1)
 * Range:     u8 n <= 23, u16 n <= 48, u32 n <= 128, u64 n <= 398,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 5) return 0;

    /* Allocate array for partition numbers up to n */
    seq_t *p = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    if (!p) return 0;  /* allocation failure */

    /* initialize */
    for (idx_t i = 0; i <= n; ++i) p[i] = 0;
    p[0] = 1;

    /* dynamic programming to compute p[0..n] */
    for (idx_t part = 1; part <= n; ++part) {
        for (idx_t sum = part; sum <= n; ++sum) {
            p[sum] += p[sum - part];
        }
    }

    /* compute the final sum */
    seq_t result = 0;
    for (idx_t k = 1; k <= n / 5; ++k) {
        idx_t idx1 = n - 5 * k;
        seq_t term1 = p[idx1];

        seq_t term2 = 0;
        if (n >= 6 * k) term2 = p[n - 6 * k];

        result += (seq_t)k * (term1 - term2);
    }

    free(p);
    return result;
}
