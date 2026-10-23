/*
 * A136167  a(n) = the number of divisors of n that are each not coprime to
 *          any adjacent (previous or following) divisor of n.
 * https://oeis.org/A136167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors of n that are not coprime to any
 *            adjacent divisor in ascending order
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

/* Compare two idx_t values for qsort */
static int cmp_idx_t(const void *p1, const void *p2)
{
    const idx_t *a = (const idx_t *)p1;
    const idx_t *b = (const idx_t *)p2;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

/* Euclidean GCD for idx_t */
static inline idx_t gcd_idx_t(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* safety for index 0 */

    /* For n <= 10 000 the number of divisors never exceeds 256. */
    idx_t divisors[256];
    size_t dcnt = 0;

    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            divisors[dcnt++] = i;
            if (i != n / i) divisors[dcnt++] = n / i;
        }
    }

    qsort(divisors, dcnt, sizeof(idx_t), cmp_idx_t);

    seq_t result = 0;
    for (size_t i = 0; i < dcnt; ++i) {
        bool bad = false;
        if (i > 0 && gcd_idx_t(divisors[i], divisors[i - 1]) == 1)
            bad = true;
        if (!bad && i + 1 < dcnt && gcd_idx_t(divisors[i], divisors[i + 1]) == 1)
            bad = true;
        if (!bad)
            ++result;
    }
    return result;
}
