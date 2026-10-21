/*
 * A230292  a(n) = Sum_{i=1..n} d(24*i+9) - Sum_{i=1..n} d(6*i+3), where d(n)
 *          = A000005(n).
 * https://oeis.org/A230292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..n} d(24*i+9) - Sum_{i=1..n} d(6*i+3)
 * Range:     u8 n <= 218, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where d(m) is the number of divisors of m (A000005). */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)                 /* safety; OEIS offset starts at 1 */
        return 0;

    /* largest argument needed for the divisor sieve */
    idx_t valmax = 24 * n + 9;

    /* Allocate an array of divisor counts (0‑initialised). */
    seq_t *divcnt = (seq_t *)malloc((size_t)(valmax + 1) * sizeof(seq_t));
    if (!divcnt)               /* allocation failure – unlikely for all test cases */
        return 0;

    for (idx_t i = 0; i <= valmax; ++i)
        divcnt[i] = 0;

    /* Standard divisor‑sieve: each i adds one to all its multiples. */
    for (idx_t i = 1; i <= valmax; ++i)
    {
        for (idx_t j = i; j <= valmax; j += i)
            ++divcnt[j];
    }

    /* Compute the required cumulative sum. */
    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t hi = divcnt[24 * i + 9];
        seq_t lo = divcnt[6 * i + 3];
        sum += hi - lo;
    }

    free(divcnt);
    return sum;
}
