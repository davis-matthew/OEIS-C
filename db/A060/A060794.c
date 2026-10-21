/*
 * A060794  Difference between upper and lower central divisors of n.
 * https://oeis.org/A060794
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = difference between upper and lower central divisors of n
 *            * (computed by enumerating all divisors, sorting them, then
 *            taking the two * middle elements in the list)
 * Range:     u8 n <= 256, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_DIVISORS 256   /* safe upper bound for n <= 10 000 */

static int cmp_idx(const void *a, const void *b)
{
    idx_t x = *(const idx_t *)a;
    idx_t y = *(const idx_t *)b;
    return (x > y) - (x < y);
}

seq_t a(idx_t n)
{
    /* Handle trivial case */
    if (n < 2) return (seq_t)0;

    idx_t divisors[MAX_DIVISORS];
    size_t cnt = 0;

    /* Compute floor(sqrt(n)) by a simple loop (safe for small n) */
    idx_t sqrt_n = 0;
    while ((sqrt_n + 1) * (sqrt_n + 1) <= n) {
        ++sqrt_n;
    }

    for (idx_t i = 1; i <= sqrt_n; ++i) {
        if (n % i == 0) {
            divisors[cnt++] = i;
            if (i != n / i) {
                divisors[cnt++] = n / i;
            }
        }
    }

    /* If for some unexpected reason we have fewer than 2 divisors, return 0 */
    if (cnt < 2) return (seq_t)0;

    /* Sort the divisor list */
    qsort(divisors, cnt, sizeof(idx_t), cmp_idx);

    /* Indices of the lower and upper central divisors */
    size_t lower_idx = (cnt / 2) - 1;
    size_t upper_idx = lower_idx + 1;

    seq_t lower = (seq_t)divisors[lower_idx];
    seq_t upper = (seq_t)divisors[upper_idx];

    return upper - lower;
}
