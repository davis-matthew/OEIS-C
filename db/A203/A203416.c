/*
 * A203416  a(n) = A203415(n+1)/A203415(n).
 * https://oeis.org/A203416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{i=1..n} (c(n+1) - c(i)), where c(k) is the k-th
 *            nonprime (A018252: 1, 4, 6, 8, 9, 10, 12, ...); equivalently
 *            A203415(n+1)/A203415(n)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 306 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static bool np_is_prime(uint64_t x)
{
    if (x < 2) return false;
    for (uint64_t d = 2; d * d <= x; ++d)
        if (x % d == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    size_t cnt = (size_t)n + 1;
    uint64_t *np = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!np) abort();
    size_t len = 0;
    uint64_t c = 1;
    np[len++] = 1;
    while (len < cnt) {
        do { ++c; } while (np_is_prime(c));
        np[len++] = c;
    }
    seq_t prod = (seq_t)1;
    uint64_t x = np[cnt - 1];
    for (size_t i = 0; i + 1 < cnt; ++i)
        prod *= (seq_t)(x - np[i]);
    free(np);
    return prod;
}
