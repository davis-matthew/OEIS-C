/*
 * A080805  Numbers n such that A072475(n) divides A071221(n).
 * https://oeis.org/A080805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k-th number m such that A072475(m) divides A071221(m);
 *            found by sieve + search, testing (product of the m-th block of
 *            composites) mod (their sum) == 0
 * Range:     u8 n <= 48, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    uint64_t k = (uint64_t)n;
    if (k == 0) return 0;
    uint64_t limit = 4096;
    for (;;) {
        uint8_t *comp = (uint8_t *)calloc((size_t)limit + 1, 1);
        if (!comp) return 0;
        for (uint64_t i = 2; i * i <= limit; ++i) {
            if (!comp[i]) {
                for (uint64_t j = i * i; j <= limit; j += i) comp[j] = 1;
            }
        }
        uint64_t cur = 4;
        uint64_t found = 0;
        bool ok = true;
        uint64_t m;
        for (m = 1; ; ++m) {
            /* collect m composites starting from cur; first pass: sum */
            uint64_t start = cur;
            uint64_t cnt = 0, sum = 0, num = cur;
            while (cnt < m && num <= limit) {
                if (comp[num]) { sum += num; ++cnt; }
                ++num;
            }
            if (cnt < m) { ok = false; break; }
            uint64_t p = 1 % sum;
            cnt = 0;
            for (uint64_t x = start; cnt < m; ++x) {
                if (comp[x]) {
                    ++cnt;
                    p = (uint64_t)(((unsigned __int128)p * (x % sum)) % sum);
                }
            }
            cur = num;
            if (p == 0) {
                if (++found == k) break;
            }
        }
        free(comp);
        if (ok) return (seq_t)m;
        limit *= 4;
    }
}
