/*
 * A140225  a(n) = number of m's among (d(1),d(2),...,d(n)), where m is the
 *          maximum value of (d(1),d(2),...,d(n)) and d(n) is the number of
 *          divisors of n.
 * https://oeis.org/A140225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of indices i (1 ≤ i ≤ n) such that d(i) =
 *            max{d(1), d(2), …, d(n)}, where d(k) is the number of divisors
 *            of k
 * Range:     u8 n <= 9698, u16 n <= 9698, u32 n <= 9698, u64 n <= 9698,
 *            u128 n <= 9698, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but avoid allocation */

    /* Allocate divisor count array (indices 0..n, 0 unused) */
    seq_t *d = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!d) return 0;              /* allocation failure – return 0 */

    for (idx_t i = 0; i <= n; ++i)
        d[i] = 0;

    /* Simple sieve to count divisors */
    for (idx_t i = 1; i <= n; ++i)
        for (idx_t j = i; j <= n; j += i)
            ++d[j];

    seq_t max_d = 0;
    seq_t count  = 0;
    for (idx_t i = 1; i <= n; ++i) {
        if (d[i] > max_d) {
            max_d = d[i];
            count = 1;
        } else if (d[i] == max_d) {
            ++count;
        }
    }

    free(d);
    return count;
}
