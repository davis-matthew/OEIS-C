/*
 * A126212  a(n) = sum of the divisors of n which equal any d(k) for 1<=k<=n,
 *          where d(k) is the number of positive divisors of k.
 * https://oeis.org/A126212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of the divisors of n that are in the set {d(k) | 1 <=
 *            k <= n}, where d(k) is the number of positive divisors of k
 * Range:     u8 n <= 1369, u16 n <= 9991, u32 n <= 9991, u64 n <= 9991,
 *            u128 n <= 9991, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;                   /* sequence starts at n=1 */

    /* --- 1. Compute divisor counts d[1..n] --- */
    /* Allocate array of seq_t of length n+1 */
    seq_t *d = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!d) return 0;

    d[0] = 0;
    for (idx_t i = 1; i <= n; ++i) d[i] = 0;

    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = i; j <= n; j += i) {
            d[j] += (seq_t)1;
        }
    }

    /* --- 2. Find maximum divisor count among 1..n to size the presence array --- */
    seq_t max_d = 0;
    for (idx_t i = 1; i <= n; ++i) {
        if (d[i] > max_d) max_d = d[i];
    }
    size_t mark_size = (size_t)max_d + 1;   /* indices 0..max_d */

    /* Allocate presence array for divisor counts that appear */
    bool *mark = (bool *)malloc(mark_size * sizeof(bool));
    if (!mark) {
        free(d);
        return 0;
    }
    for (size_t i = 0; i < mark_size; ++i) mark[i] = false;

    /* Mark all divisor counts that occur among 1..n */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t val = d[i];
        if ((size_t)val < mark_size) mark[(size_t)val] = true;
    }

    /* --- 3. Sum the divisors of n that are marked --- */
    seq_t sum = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            seq_t d1 = (seq_t)i;                /* divisor i */
            seq_t d2 = (seq_t)(n / i);          /* divisor n/i */

            if ((size_t)d1 < mark_size && mark[(size_t)d1]) sum += d1;

            if (d2 != d1 && (size_t)d2 < mark_size && mark[(size_t)d2]) {
                sum += d2;
            }
        }
    }

    /* --- 4. Clean up and return result --- */
    free(d);
    free(mark);
    return sum;
}
