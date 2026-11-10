/*
 * A081375  a(n) is the least number k such that A081373(k) = n.
 * https://oeis.org/A081375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k such that #{ 1 <= m <= k : phi(m) = phi(k) } = n;
 *            computed by a doubling-limit totient sieve and a scan counting
 *            phi values
 * Range:     u8 n <= 16, u16 n <= 197, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 65 terms through n = 923 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;
    uint64_t L = 1024;
    while (L <= 4000000000ULL) {
        uint32_t *phi = (uint32_t*)malloc((L + 1) * sizeof(uint32_t));
        uint32_t *cnt = (uint32_t*)malloc((L + 1) * sizeof(uint32_t));
        if (!phi || !cnt) { free(phi); free(cnt); return 0; }
        for (uint64_t i = 0; i <= L; ++i) { phi[i] = (uint32_t)i; cnt[i] = 0; }
        for (uint64_t p = 2; p <= L; ++p) {
            if (phi[p] == (uint32_t)p) {  /* p is prime (untouched) */
                for (uint64_t j = p; j <= L; j += p)
                    phi[j] -= phi[j] / (uint32_t)p;
            }
        }
        for (uint64_t k = 1; k <= L; ++k) {
            uint32_t c = ++cnt[phi[k]];
            if ((idx_t)c == n) {
                free(phi); free(cnt);
                return (seq_t)k;
            }
        }
        free(phi); free(cnt);
        L *= 2;
    }
    return 0;
}
