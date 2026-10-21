/*
 * A225529  Number of distinct products i*j over all pairs (i,j) with i,j >= 0
 *          and i+j <= n and gcd(i,j) <= 1.
 * https://oeis.org/A225529
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of distinct products i*j with i,j ≥ 0, i + j ≤ n
 *            and gcd(i,j) ≤ 1
 * Range:     u8 n <= 46, u16 n <= 400, u32 n <= 400, u64 n <= 400,
 *            u128 n <= 400, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Euclidean algorithm for idx_t (unsigned 64‑bit) */
static inline idx_t gcd_idx(idx_t a, idx_t b) {
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* Quick answer for n = 0: only product 0 is possible */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Maximum possible product: floor(n^2/4)  (i = j = n/2) */
    idx_t half = n / 2;
    idx_t max_prod = half * (n - half);
    size_t words = (size_t)(max_prod >> 6) + 1;   /* 64 bits per word */

    uint64_t *bits = (uint64_t *)malloc(words * sizeof(uint64_t));
    if (!bits) {                    /* allocation failed – return 0 */
        return (seq_t)0;
    }
    memset(bits, 0, words * sizeof(uint64_t));

    seq_t count = 0;   /* distinct products found */

    for (idx_t i = 0; i <= n; ++i) {
        idx_t max_j = n - i;
        for (idx_t j = 0; j <= max_j; ++j) {
            if (gcd_idx(i, j) > (idx_t)1) {
                continue;           /* skip non‑coprime pairs */
            }
            idx_t prod = i * j;      /* product fits in idx_t for given limits */
            size_t word = prod >> 6;
            uint64_t mask = (uint64_t)1 << (prod & 63);
            if ((bits[word] & mask) == 0) {
                bits[word] |= mask;
                ++count;
            }
        }
    }

    free(bits);

    return count;
}
