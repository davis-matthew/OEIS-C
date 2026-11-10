/*
 * A081376  a(n) is the least number such that A067003(a(n)) = n.
 * https://oeis.org/A081376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least m such that #{k <= m : omega(k) = omega(m)} = n
 *            (omega = number of distinct prime factors); computed by sieving
 *            omega and scanning with per-class counters
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Validated: partial, pass, 68 terms through n = 68 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    idx_t limit = 4 * n + 64;
    for (;;) {
        unsigned char *om = (unsigned char *)calloc((size_t)limit + 1, 1);
        if (!om) return 0;
        for (idx_t p = 2; p <= limit; ++p) {
            if (om[p] == 0) {
                for (idx_t j = p; j <= limit; j += p) om[j]++;
            }
        }
        idx_t cnt[64];
        for (int i = 0; i < 64; ++i) cnt[i] = 0;
        for (idx_t m = 1; m <= limit; ++m) {
            if (++cnt[om[m]] == n) {
                free(om);
                return (seq_t)m;
            }
        }
        free(om);
        limit *= 2;
    }
}
