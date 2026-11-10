/*
 * A238247  Numbers m such that A072219(m) = 5.
 * https://oeis.org/A238247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th m with A072219(m)=5, i.e. m = 2^k1 - 2^k2 + 2^k3 -
 *            2^k4 + 2^k5, k1>k2>k3>k4>k5>=0; enumerate by k1 (C(k1,4) terms
 *            each), sort, select
 * Range:     u8 n <= 126, u16 n <= 6188, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 8664 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static int A238247_cmp(const void *x, const void *y)
{
    uint64_t p = *(const uint64_t *)x, q = *(const uint64_t *)y;
    return (p > q) - (p < q);
}

static uint64_t A238247_c4(uint64_t K)
{
    return K * (K - 1) / 2 * (K - 2) / 3 * (K - 3) / 4;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    uint64_t r = (uint64_t)n;
    unsigned K = 4;
    for (;;) {
        uint64_t c = A238247_c4(K);
        if (r > c) {
            r -= c;
            K++;
        } else
            break;
    }
    uint64_t cnt = A238247_c4(K);
    uint64_t *arr = (uint64_t *)malloc(sizeof(uint64_t) * (size_t)cnt);
    size_t pos = 0;
    for (unsigned x = 3; x < K; x++)
        for (unsigned y = 2; y < x; y++)
            for (unsigned z = 1; z < y; z++)
                for (unsigned w = 0; w < z; w++) {
                    uint64_t v = ((uint64_t)1 << K) - ((uint64_t)1 << x)
                               + ((uint64_t)1 << y) - ((uint64_t)1 << z)
                               + ((uint64_t)1 << w);
                    arr[pos++] = v;
                }
    qsort(arr, pos, sizeof(uint64_t), A238247_cmp);
    uint64_t res = arr[r - 1];
    free(arr);
    return (seq_t)res;
}
