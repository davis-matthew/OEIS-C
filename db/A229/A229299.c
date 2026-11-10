/*
 * A229299  Number of terms in A191677 less than or equal to 10^n.
 * https://oeis.org/A229299
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more, hard
 * Formula:   a(n) = #{m <= 10^n : m in A191677}, where m is in A191677 iff m
 *            = 1, or 4 | m, or m is odd and no prime p | m has (p-1) | (m-1).
 *            Computed by a sieve over smallest prime factors
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Validated: partial, pass, 7 terms through n = 7 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    uint64_t N = 1;
    for (idx_t i = 0; i < n; i++) N *= 10;

    uint32_t *spf = (uint32_t *)malloc((size_t)(N + 1) * sizeof(uint32_t));
    if (spf == NULL) exit(1);
    for (uint64_t i = 0; i <= N; i++) spf[i] = (uint32_t)i;
    for (uint64_t i = 2; i * i <= N; i++) {
        if (spf[i] == i) {
            for (uint64_t j = i * i; j <= N; j += i)
                if (spf[j] == j) spf[j] = (uint32_t)i;
        }
    }

    uint64_t count = 1; /* m = 1 */
    for (uint64_t m = 2; m <= N; m++) {
        if ((m & 3) == 0) { count++; continue; }
        if ((m & 1) == 0) continue;
        uint64_t t = m;
        bool ok = true;
        while (t > 1) {
            uint64_t p = spf[t];
            while (t % p == 0) t /= p;
            if ((m - 1) % (p - 1) == 0) { ok = false; break; }
        }
        if (ok) count++;
    }
    free(spf);
    return (seq_t)count;
}
