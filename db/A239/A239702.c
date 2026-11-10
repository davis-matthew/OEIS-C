/*
 * A239702  Triangle read by rows: T(n,k) = A239682(n)/(A239682(k)*
 *          A239682(n-k)).
 * https://oeis.org/A239702
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = F(n)/(F(k)*F(n-k)), F(m) = Prod_{i=1..m} A173557(i),
 *            F(0)=1; a(N) = T(r,k) with N = r(r+1)/2 + k. Computed exactly
 *            via prime exponents of F
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Validated: partial, pass, 80 terms through n = 79 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static void T239702_addf(uint64_t *arr, uint64_t y)
{
    for (uint64_t q = 2; q <= y / q; ++q) {
        while (y % q == 0) {
            arr[q]++;
            y /= q;
        }
    }
    if (y > 1)
        arr[y]++;
}

/* arr[q] = exponent of prime q in F(m) */
static void T239702_expo(uint64_t m, uint64_t *arr)
{
    for (uint64_t i = 2; i <= m; ++i) {
        uint64_t x = i;
        for (uint64_t p = 2; p <= x / p; ++p) {
            if (x % p == 0) {
                T239702_addf(arr, p - 1);
                do {
                    x /= p;
                } while (x % p == 0);
            }
        }
        if (x > 1)
            T239702_addf(arr, x - 1);
    }
}

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    uint64_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= N)
        r++;
    uint64_t k = N - r * (r + 1) / 2;

    size_t sz = (size_t)r + 2;
    uint64_t *er  = (uint64_t *)calloc(sz, sizeof(uint64_t));
    uint64_t *ek  = (uint64_t *)calloc(sz, sizeof(uint64_t));
    uint64_t *erk = (uint64_t *)calloc(sz, sizeof(uint64_t));

    T239702_expo(r, er);
    T239702_expo(k, ek);
    T239702_expo(r - k, erk);

    seq_t res = 1;
    for (uint64_t q = 2; q <= r; ++q) {
        uint64_t e = er[q] - ek[q] - erk[q];
        for (uint64_t j = 0; j < e; ++j)
            res *= (seq_t)q;
    }

    free(er);
    free(ek);
    free(erk);
    return res;
}
