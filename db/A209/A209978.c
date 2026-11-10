/*
 * A209978  a(n) = A196227(n)/2.
 * https://oeis.org/A209978
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*Sum_{k=1..n} phi(k) - n - 1 for n > 0, a(0) = 0 (i.e.
 *            A196227(n)/2)
 * Range:     u8 n <= 20, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 51 terms through n = 50 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    size_t N = (size_t)n;
    uint32_t *phi = (uint32_t *)malloc((N + 1) * sizeof(uint32_t));
    if (!phi) abort();
    for (size_t i = 0; i <= N; ++i) phi[i] = (uint32_t)i;
    for (size_t i = 2; i <= N; ++i) {
        if (phi[i] == (uint32_t)i) {
            for (size_t j = i; j <= N; j += i)
                phi[j] -= phi[j] / (uint32_t)i;
        }
    }
    seq_t sum = 0;
    for (size_t i = 1; i <= N; ++i) sum += (seq_t)phi[i];
    free(phi);
    return 2 * sum - (seq_t)n - 1;
}
