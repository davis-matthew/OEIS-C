/*
 * A128779  a(n) = denominator of Product_{k=1..n} (1 + {n/k}), where {x} is
 *          the fractional part of x, {x} = x - floor(x).
 * https://oeis.org/A128779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = denominator of Product_{k=1..n} (1 + {n/k})
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 22, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Validated: partial, pass, 28 terms through n = 28 (2026-11-01)
 * Helpers:   build_spf, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where {x} = x - floor(x) is the fractional part of x. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/build_spf.h"
#include "../utils/ipow_i32.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

static idx_t *g_spf       = NULL;           /* array of smallest prime factor */
static idx_t  g_spf_limit = 0;             /* maximum n for which spf is valid */

static void ensure_spf(idx_t limit)
{
    if (limit <= g_spf_limit) return;
    free(g_spf);
    build_spf(limit, &g_spf);
    g_spf_limit = limit;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                  /* undefined for n < 1, safety */

    idx_t limit = 2 * n;                  /* we need spf up to 2n-1 */
    ensure_spf(limit);

    /* diff[p] = exponent of p in denominator - exponent in numerator */
    int *diff = (int*)calloc(limit + 1, sizeof(int));
    if (!diff) return 0;                  /* allocation failure */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* factor denominator term k */
        idx_t x = k;
        while (x > 1)
        {
            idx_t p = g_spf[x];
            int e = 0;
            do { ++e; x /= p; } while (x % p == 0);
            diff[p] += e;
        }

        /* factor numerator term k + (n % k) */
        idx_t r    = n % k;
        idx_t prod = k + r;
        x = prod;
        while (x > 1)
        {
            idx_t p = g_spf[x];
            int e = 0;
            do { ++e; x /= p; } while (x % p == 0);
            diff[p] -= e;
        }
    }

    seq_t denom = 1;
    for (idx_t p = 2; p <= limit; ++p)
    {
        int d = diff[p];
        if (d > 0)
            denom = seq_mul(denom, ipow_i32((seq_t)p, d));
    }

    free(diff);
    return denom;
}
