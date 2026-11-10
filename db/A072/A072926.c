/*
 * A072926  a(n) = Sum_{k=1..n} A051699(k).
 * https://oeis.org/A072926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} A051699(k), A051699(k) = distance from k to
 *            nearest prime; computed with a sieve and a linear scan
 * Range:     u8 n <= 175, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 90 terms through n = 9782 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t extra = 1000;
    unsigned char *comp;
    idx_t L;
    for (;;) {
        L = n + extra;
        comp = (unsigned char *)calloc((size_t)L + 1, 1);
        if (!comp) return 0;
        comp[0] = 1;
        if (L >= 1) comp[1] = 1;
        for (idx_t i = 2; i * i <= L; i++) {
            if (!comp[i]) {
                for (idx_t j = i * i; j <= L; j += i) comp[j] = 1;
            }
        }
        /* need a prime >= n */
        bool ok = false;
        for (idx_t j = (n < 2 ? 2 : n); j <= L; j++) {
            if (!comp[j]) { ok = true; break; }
        }
        if (ok) break;
        free(comp);
        extra *= 2;
    }

    seq_t sum = 0;
    bool have_last = false;
    idx_t last = 0;
    idx_t nxt = 2;
    for (idx_t k = 1; k <= n; k++) {
        if (k > nxt) {
            have_last = true;
            last = nxt;
            nxt = k;
            while (comp[nxt]) nxt++;
        }
        idx_t d = nxt - k;
        if (have_last && k - last < d) d = k - last;
        sum += (seq_t)d;
    }
    free(comp);
    return sum;
}
