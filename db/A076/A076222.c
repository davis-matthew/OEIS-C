/*
 * A076222  Number of different values in the n-th row of A076221.
 * https://oeis.org/A076222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{ A(n,k) : 1 <= k <= n }|, where A(n,k) = #{ x <= n :
 *            gcd(x,k) = 1, x != k }; computed by direct count over row n
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Validated: partial, pass, 76 terms through n = 76 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t A076222_gcd(uint64_t u, uint64_t v)
{
    while (v != 0) {
        uint64_t t = u % v;
        u = v;
        v = t;
    }
    return u;
}

seq_t a(idx_t n)
{
    size_t N = (size_t)n;
    unsigned char *seen = (unsigned char *)calloc(N + 2, 1);
    if (!seen) return 0;
    seq_t distinct = 0;
    for (uint64_t k = 1; k <= (uint64_t)n; k++) {
        uint64_t cnt = 0;
        for (uint64_t x = 1; x <= (uint64_t)n; x++) {
            if (x == k) continue;
            if (A076222_gcd(x, k) == 1) cnt++;
        }
        if (!seen[cnt]) {
            seen[cnt] = 1;
            distinct++;
        }
    }
    free(seen);
    return distinct;
}
