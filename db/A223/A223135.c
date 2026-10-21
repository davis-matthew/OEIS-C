/*
 * A223135  Number of distinct sums i + j + k with i, j, k >= 0, i*j*k = n and
 *          gcd(i,j,k) <= 1.
 * https://oeis.org/A223135
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct sums i + j + k with * i, j, k >= 0,
 *            i*j*k = n and gcd(i, j, k) <= 1. * * For n>0 all i,j,k are
 *            positive and must satisfy gcd(i,j,k)=1. * For n==0 only the
 *            triple (0,0,0) is counted, giving a(0)=1
 * Range:     u8 n <= 9410, u16 n <= 9410, u32 n <= 9410, u64 n <= 9410,
 *            u128 n <= 9410, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Euclidean algorithm for 64‑bit unsigned integers */
static inline uint64_t gcd_u64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    idx_t nn = n;
    /* Upper bound for i+j+k is nn+2 (when i=j=1,k=nn). */
    size_t max_sum = (size_t)nn + 2;
    bool *seen = (bool *)calloc(max_sum + 1, sizeof(bool));
    if (!seen) return (seq_t)0; /* memory allocation failed */

    size_t count = 0;

    for (idx_t i = 1; i * i * i <= nn; ++i) {
        if (nn % i != 0) continue;
        idx_t m = nn / i;
        for (idx_t j = 1; j * j <= m; ++j) {
            if (m % j != 0) continue;
            idx_t k = m / j;
            /* gcd(i,j,k) == 1 */
            uint64_t g = gcd_u64(i, j);
            g = gcd_u64(g, k);
            if (g != 1) continue;
            idx_t sum = i + j + k;
            if (!seen[sum]) {
                seen[sum] = true;
                ++count;
            }
        }
    }

    free(seen);
    return (seq_t)count;
}
