/*
 * A000452  The greedy sequence of integers which avoids 3-term geometric
 *          progressions.
 * https://oeis.org/A000452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, changed
 * Formula:   a(n) = n-th positive integer whose prime-factorization exponents
 *            all lie in A005836 (base-3 digits only 0 or 1); computed by a
 *            smallest-prime-factor sieve over a growing limit
 * Range:     u8 n <= 186, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   exp_ok
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/exp_ok.h"

seq_t a(idx_t n) {
    static uint64_t *terms = NULL;
    static idx_t cnt = 0;
    static uint64_t lim = 0;

    if (n < 1) return 0;
    if (n <= cnt) return (seq_t)terms[n - 1];

    uint64_t newlim = lim ? lim : 64;
    while (newlim < 2 * (uint64_t)n + 16) newlim *= 2;

    for (;;) {
        uint32_t *spf = (uint32_t *)calloc((size_t)newlim + 1, sizeof(uint32_t));
        for (uint64_t i = 2; i <= newlim; i++) {
            if (spf[i] == 0) {
                for (uint64_t j = i; j <= newlim; j += i)
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
            }
        }
        uint64_t *t = (uint64_t *)malloc(sizeof(uint64_t) * ((size_t)newlim + 1));
        idx_t c = 0;
        for (uint64_t m = 1; m <= newlim; m++) {
            uint64_t x = m;
            int ok = 1;
            while (x > 1 && ok) {
                uint32_t p = spf[x];
                unsigned e = 0;
                while (x % p == 0) { x /= p; e++; }
                if (!exp_ok(e)) ok = 0;
            }
            if (ok) t[c++] = m;
        }
        free(spf);
        free(terms);
        terms = t;
        cnt = c;
        lim = newlim;
        if (cnt >= n) break;
        newlim *= 2;
    }
    return (seq_t)terms[n - 1];
}
