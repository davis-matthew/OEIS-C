/*
 * A082468  a(n) is the smallest positive integer k such that A082467(k)=n.
 * https://oeis.org/A082468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k such that A082467(k) = n, where A082467(k) =
 *            least d>0 with k-d and k+d both prime; found by sieve and search
 * Range:     u8 n <= 18, u16 n <= 181, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 70 terms through n = 963 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t L = 4 * n + 64;
    for (;;) {
        idx_t M = L + n + 2;
        unsigned char *c = (unsigned char *)calloc((size_t)M + 1, 1);
        if (!c) return 0;
        c[0] = 1; c[1] = 1;
        for (idx_t i = 2; i * i <= M; ++i)
            if (!c[i])
                for (idx_t j = i * i; j <= M; j += i) c[j] = 1;
        for (idx_t k = n + 2; k <= L; ++k) {
            if (c[k - n] || c[k + n]) continue;
            bool ok = true;
            for (idx_t d = 1; d < n; ++d) {
                if (!c[k - d] && !c[k + d]) { ok = false; break; }
            }
            if (ok) {
                free(c);
                return (seq_t)k;
            }
        }
        free(c);
        L *= 4;
    }
}
