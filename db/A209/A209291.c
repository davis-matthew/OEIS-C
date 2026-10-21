/*
 * A209291  Sum of the refactorable numbers less than or equal to n.
 * https://oeis.org/A209291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=1..n} i if i % d(i) == 0 where d(i) = number of
 *            divisors of i
 * Range:     u8 n <= 59, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* offset is 1, but return 0 for safety */

    /* allocate array for divisor counts d[1..n] */
    idx_t size = n + 1;
    unsigned int *d = (unsigned int *)calloc(size, sizeof(unsigned int));
    if (!d) return 0;                          /* allocation failed: return 0 */

    /* sieve to compute divisor counts */
    for (idx_t i = 1; i <= n; ++i)
        for (idx_t j = i; j <= n; j += i)
            d[j]++;

    /* sum the refactorable numbers */
    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i)
        if (i % d[i] == 0)
            sum += (seq_t)i;

    free(d);
    return sum;
}
