/*
 * A064603  Partial sums of A001158: Sum_{j=1..n} sigma_3(j).
 * https://oeis.org/A064603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=1..n} sigma_3(j) = partial sums of cubes of
 *            divisors. Recurrence: a(1) = 1, a(n) = a(n-1) + sigma_3(n), n >=
 *            2
 * Range:     u8 n <= 5, u16 n <= 21, u32 n <= 354, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static idx_t cache_n = 0;      /* largest n for which a_values are computed */
    static seq_t *a_values = NULL; /* a(0)=0, a(1), a(2), ... */

    if (n == 0) return (seq_t)0;   /* undefined for n<1, return 0 */

    /* Ensure the cache is large enough */
    if (n > cache_n) {
        /* resize a_values array */
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(a_values, new_size * sizeof(seq_t));
        if (!tmp) exit(1);          /* allocation failure */
        a_values = tmp;

        /* Initialize a_values[0] */
        if (cache_n == 0) a_values[0] = (seq_t)0;

        /* Compute missing entries from cache_n+1 up to n */
        for (idx_t i = cache_n + 1; i <= n; ++i) {
            seq_t sigma = (seq_t)0;
            /* sum cubes of divisors of i */
            for (idx_t d = 1; d <= i / d; ++d) { /* d*d <= i without overflow */
                if (i % d == 0) {
                    seq_t d1 = (seq_t)d;
                    sigma += d1 * d1 * d1;          /* d^3 */
                    if (d != i / d) {             /* add complementary divisor */
                        seq_t d2 = (seq_t)(i / d);
                        sigma += d2 * d2 * d2;
                    }
                }
            }
            a_values[i] = a_values[i - 1] + sigma;
        }
        cache_n = n;
    }

    return a_values[n];
}
