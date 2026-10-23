/*
 * A124254  Forest-and-trees problem: number of trees visible. (See Comments.)
 * https://oeis.org/A124254
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of coprime positive integer pairs (x,y) with 1 ≤ x
 *            ≤ y and x² + y² < n². The function counts such pairs for n ≥ 2
 *            using a smallest‑prime‑factor sieve and inclusion–exclusion
 * Range:     u8 n <= 32, u16 n <= 523, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>


/* floor(sqrt(v)) for non‑negative v, using 64‑bit arithmetic. */
static inline idx_t isqrt_u64(idx_t v) {
    idx_t lo = 0, hi = v + 1;
    while (lo + 1 < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        uint64_t sq = (uint64_t)mid * (uint64_t)mid;
        if (sq <= v)
            lo = mid;
        else
            hi = mid;
    }
    return lo;
}

/* Smallest‑prime‑factor table.  Built up to the largest n that has been
   requested. */
static idx_t *spf = NULL;
static idx_t spf_limit = 0;

static void ensure_spf(idx_t n) {
    if (n <= spf_limit) return;
    free(spf);
    spf = (idx_t*)calloc(n + 1, sizeof(idx_t));
    if (!spf) return;            /* allocation failed – unlikely in test harness */
    spf[0] = 0;
    spf[1] = 1;
    for (idx_t i = 2; i <= n; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if (i > n / i) continue;
            for (idx_t j = i * i; j <= n; j += i)
                if (spf[j] == 0)
                    spf[j] = i;
        }
    }
    spf_limit = n;
}

seq_t a(idx_t n) {
    if (n < 2) return 0; /* undefined for n < 2, return 0 */
    ensure_spf(n - 1);   /* factors for all y < n */

    seq_t res = 0;
    uint64_t nn = (uint64_t)n * (uint64_t)n;          /* n² */

    for (idx_t y = 1; y < n; ++y) {
        uint64_t yy = (uint64_t)y * (uint64_t)y;
        uint64_t rem = nn - yy;                     /* n² – y² */
        if (rem <= 1) continue;                     /* no x > 0 satisfies x² + y² < n² */

        uint64_t t = rem - 1;                       /* strict inequality */
        idx_t maxX = isqrt_u64(t);
        if (maxX > y) maxX = y;                     /* x ≤ y constraint */
        if (maxX == 0) continue;

        seq_t count = (seq_t)maxX;                  /* initial count: all 1 ≤ x ≤ maxX */

        /* inclusion–exclusion over distinct prime factors of y */
        idx_t tmp = y;
        idx_t primes[12];
        int pc = 0;
        while (tmp > 1 && pc < (int)(sizeof(primes)/sizeof(primes[0]))) {
            idx_t p = spf[tmp];
            primes[pc++] = p;
            while (tmp % p == 0) tmp /= p;
        }

        int subsets = 1 << pc;
        for (int mask = 1; mask < subsets; ++mask) {
            uint64_t prod = 1;
            for (int k = 0; k < pc; ++k) {
                if (mask & (1 << k)) {
                    prod *= (uint64_t)primes[k];
                    if (prod > (uint64_t)maxX) break;
                }
            }
            if (prod > (uint64_t)maxX) continue;
            seq_t term = (seq_t)(maxX / prod);
            if (__builtin_popcount((unsigned)mask) & 1)
                count -= term;
            else
                count += term;
        }
        res += count;
    }
    return res;
}
