/*
 * A218993  Numerator of the least reduced fraction b/c > 1 using divisors b
 *          and c of n.
 * https://oeis.org/A218993
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   cmp_ratio
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A218993:  Numerator of the least reduced fraction b/c > 1 with b,c|n and b*c | n. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/cmp_ratio.h"

/* Small helper: greatest common divisor for unsigned 64‑bit integers. */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;   /* undefined for offset 1 */

    /* Find integer square root of n by division to avoid overflow. */
    idx_t limit = 0;
    for (idx_t i = 1; i <= n / i; ++i) limit = i;

    /* Allocate space for all divisors: at most 2·limit + 1 entries. */
    size_t capacity = (size_t)limit * 2 + 1;
    idx_t *divs = (idx_t *)malloc(capacity * sizeof(idx_t));
    size_t sz = 0;
    for (idx_t i = 1; i <= limit; ++i) {
        if (n % i == 0) {
            divs[sz++] = i;
            idx_t j = n / i;
            if (j != i) divs[sz++] = j;
        }
    }

    /* Start with the trivial pair (n,1). */
    idx_t bestB = n, bestC = 1;

    /* Enumerate all pairs (b,c) with b > c. */
    for (size_t ib = 0; ib < sz; ++ib) {
        idx_t b = divs[ib];
        for (size_t ic = 0; ic < sz; ++ic) {
            idx_t c = divs[ic];
            if (b <= c) continue;                /* need b > c */
            if (gcd_idx(b, c) != 1) continue;    /* must be reduced */

            if (b > n / c) continue;             /* product would exceed n */
            idx_t prod = b * c;
            if (n % prod != 0) continue;         /* product must divide n */

            if (cmp_ratio(b, c, bestB, bestC) < 0) {
                bestB = b;
                bestC = c;
            }
        }
    }

    free(divs);
    return (seq_t)bestB;
}
