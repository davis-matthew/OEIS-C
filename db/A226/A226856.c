/*
 * A226856  Numbers A226770((prime(n))^2-1).
 * https://oeis.org/A226856
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A226770(prime(n)^2 - 1); computed by running A226770's
 *            divisor-closure search with a smallest-prime-factor sieve
 * Range:     u8 n <= 27, u16 n <= 226, u32 n <= 400, u64 n <= 400,
 *            u128 n <= 400, big any n
 * Validated: standard, pass, 83 terms through n = 395 (2026-11-07)
 * Helpers:   nth_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/nth_prime.h"

static uint64_t A226856_closure(uint64_t N)
{
    /* Every number reached is N + d with d <= N, so it is at most 2N.
       Every proper divisor of such a number is at most N. */
    uint64_t M = 2 * N + 2;
    size_t half = (size_t)(M / 2 + 1);
    uint16_t *spf = (uint16_t *)calloc(half, sizeof(uint16_t)); /* spf of odd k at k>>1, 0 = prime */
    uint8_t *seen = (uint8_t *)calloc((size_t)((N + 2) / 8 + 1), 1);
    uint32_t *stk = (uint32_t *)malloc((size_t)(N + 2) * sizeof(uint32_t));
    if (!spf || !seen || !stk) { free(spf); free(seen); free(stk); return 0; }

    for (uint64_t p = 3; p * p <= M; p += 2) {
        if (spf[p >> 1] == 0) {
            for (uint64_t j = p * p; j <= M; j += 2 * p)
                if (spf[j >> 1] == 0) spf[j >> 1] = (uint16_t)p;
        }
    }

    size_t sp = 0;
    uint64_t count = 0;
    uint64_t m = N + 1;
    uint64_t divs[4096];
    for (;;) {
        /* factor m */
        uint64_t pr[32]; int ex[32]; int np = 0;
        uint64_t t = m;
        if ((t & 1) == 0) {
            int e = 0;
            while ((t & 1) == 0) { t >>= 1; ++e; }
            pr[np] = 2; ex[np] = e; ++np;
        }
        while (t > 1) {
            uint64_t q = spf[t >> 1];
            if (q == 0) q = t;
            int e = 0;
            while (t % q == 0) { t /= q; ++e; }
            pr[np] = q; ex[np] = e; ++np;
        }
        /* enumerate divisors */
        size_t nd = 1;
        divs[0] = 1;
        for (int i = 0; i < np; ++i) {
            size_t cur = nd;
            uint64_t pw = 1;
            for (int e = 1; e <= ex[i]; ++e) {
                pw *= pr[i];
                for (size_t k = 0; k < cur && nd < 4096; ++k)
                    divs[nd++] = divs[k] * pw;
            }
        }
        for (size_t k = 0; k < nd; ++k) {
            uint64_t d = divs[k];
            if (d <= 1 || d >= m || d > N) continue;
            if (!(seen[d >> 3] & (uint8_t)(1u << (d & 7)))) {
                seen[d >> 3] |= (uint8_t)(1u << (d & 7));
                stk[sp++] = (uint32_t)d;
                ++count;
            }
        }
        if (sp == 0) break;
        m = N + stk[--sp];
    }

    free(spf);
    free(seen);
    free(stk);
    return count;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    uint64_t p = nth_prime(n);
    return (seq_t)A226856_closure(p * p - 1);
}
