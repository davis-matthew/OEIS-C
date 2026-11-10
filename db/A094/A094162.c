/*
 * A094162  Where n first appears in A093320.
 * https://oeis.org/A094162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = least k such that A093320(k) = n; found by a sieve over
 *            k, doubling the limit until n appears
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Validated: partial, pass, 12 terms through n = 12 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    uint64_t limit = 1u << 16;
    for (;;) {
        uint32_t *spf = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
        uint32_t *val = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
        if (!spf || !val) {
            free(spf);
            free(val);
            return (seq_t)0;
        }
        for (uint64_t i = 0; i <= limit; ++i) spf[i] = (uint32_t)i;
        for (uint64_t i = 2; i * i <= limit; ++i) {
            if (spf[i] == i) {
                for (uint64_t j = i * i; j <= limit; j += i)
                    if (spf[j] == j) spf[j] = (uint32_t)i;
            }
        }
        val[0] = 0;
        val[1] = 1;
        uint64_t cnt = 0;
        uint64_t found = 0;
        if (n == 1) found = 1;
        for (uint64_t m = 2; m <= limit && !found; ++m) {
            uint32_t v;
            if (spf[m] == m) {
                ++cnt;
                v = val[cnt];
            } else {
                uint64_t p = spf[m];
                uint64_t r = m / p;
                while (r % p == 0) r /= p;
                v = val[p] + (r > 1 ? val[r] : 0);
            }
            val[m] = v;
            if ((idx_t)v == n) found = m;
        }
        free(spf);
        free(val);
        if (found) return (seq_t)found;
        limit *= 2;
    }
}
