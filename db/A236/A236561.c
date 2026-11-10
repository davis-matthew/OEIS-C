/*
 * A236561  Values taken by the A049820, sorted into ascending order.
 * https://oeis.org/A236561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th smallest term of the multiset {k - d(k) : k >= 1}
 *            (A049820 sorted, with repetition), found by sieving d(k) up to a
 *            bound M. Every value below M - 2*floor(sqrt(M)) - 2 is complete,
 *            because k - d(k) >= k - 2*sqrt(k) for every k
 * Range:     u8 n <= 263, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 84 terms through n = 9634 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    size_t target = (size_t)n;
    size_t M = 2 * target + 64;
    for (;;) {
        uint32_t *d = (uint32_t *)calloc(M + 1, sizeof(uint32_t));
        uint32_t *mult = (uint32_t *)calloc(M + 1, sizeof(uint32_t));
        size_t i, j, s = 0;
        while ((s + 1) * (s + 1) <= M) s++;
        size_t T = M - 2 * s - 2; /* every value < T has its full count */
        for (i = 1; i <= M; i++)
            for (j = i; j <= M; j += i) d[j]++;
        for (i = 1; i <= M; i++) mult[i - d[i]]++;
        size_t cnt = 0;
        for (i = 0; i < T; i++) {
            cnt += mult[i];
            if (cnt >= target) {
                free(d);
                free(mult);
                return (seq_t)i;
            }
        }
        free(d);
        free(mult);
        M *= 2;
    }
}
