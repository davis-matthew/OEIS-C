/*
 * A135874  Multiply the positive divisors of n in order (starting at 1). a(n)
 *          is the smallest such partial product that is >= n.
 * https://oeis.org/A135874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = smallest partial product of the positive divisors * of n
 *            in increasing order that is >= n
 * Range:     u8 n <= 48, u16 n <= 1071, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Compare two uint64_t values for qsort. */
static int cmp_uint64(const void *a, const void *b)
{
    uint64_t ua = *(const uint64_t *)a;
    uint64_t ub = *(const uint64_t *)b;
    return (ua > ub) - (ua < ub);
}

seq_t a(idx_t n)
{
    /* Find floor(sqrt(n)) without using math.h */
    idx_t sqrt_n = 0;
    for (idx_t i = 1; i <= n / i; ++i)
        sqrt_n = i;

    /* Allocate space for all divisors: at most 2*sqrt_n+2 */
    size_t cap = (size_t)sqrt_n * 2 + 2;
    uint64_t *divs = (uint64_t *)malloc(cap * sizeof(uint64_t));
    size_t count = 0;

    /* Collect divisors in arbitrary order */
    for (idx_t i = 1; i <= sqrt_n; ++i) {
        if (n % i == 0) {
            divs[count++] = (uint64_t)i;
            idx_t j = n / i;
            if (j != i) {
                divs[count++] = (uint64_t)j;
            }
        }
    }

    /* Sort divisors in increasing order */
    qsort(divs, count, sizeof(uint64_t), cmp_uint64);

    /* Compute the smallest partial product >= n */
    seq_t prod = (seq_t)1;
    for (size_t k = 0; k < count; ++k) {
        prod = prod * (seq_t)divs[k];
        if (prod >= (seq_t)n) {
            free(divs);
            return prod;
        }
    }

    free(divs);
    return prod; /* Fallback, should never be reached */
}
