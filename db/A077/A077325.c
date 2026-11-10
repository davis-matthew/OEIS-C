/*
 * A077325  Average of terms of n-th row of A077321.
 * https://oeis.org/A077325
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A077324(n)/n, where row m of A077321 holds the m smallest
 *            primes == 1 (mod m) not already used in earlier rows (simulated
 *            with a sieve, growing the sieve limit as needed)
 * Range:     u8 n <= 8, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Validated: partial, pass, 44 terms through n = 44 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint64_t *a077325_res = NULL;
static idx_t a077325_n = 0;
static uint64_t a077325_lim = 1u << 16;

static void a077325_compute(idx_t n)
{
    for (;;) {
        uint64_t lim = a077325_lim;
        unsigned char *comp = (unsigned char *)malloc((size_t)lim + 1);
        unsigned char *used = (unsigned char *)malloc((size_t)lim + 1);
        uint64_t *res = (uint64_t *)malloc(((size_t)n + 1) * sizeof(uint64_t));
        uint64_t i, j;
        idx_t m;
        int ok = 1;

        memset(comp, 0, (size_t)lim + 1);
        memset(used, 0, (size_t)lim + 1);
        comp[0] = 1;
        if (lim >= 1) comp[1] = 1;
        for (i = 2; i * i <= lim; i++)
            if (!comp[i])
                for (j = i * i; j <= lim; j += i) comp[j] = 1;

        for (m = 1; m <= n; m++) {
            uint64_t sum = 0, cnt = 0, p;
            for (p = (uint64_t)m + 1; p <= lim && cnt < m; p += m) {
                if (!comp[p] && !used[p]) {
                    used[p] = 1;
                    sum += p;
                    cnt++;
                }
            }
            if (cnt < m) { ok = 0; break; }
            res[m] = sum / m;
        }
        free(comp);
        free(used);
        if (ok) {
            free(a077325_res);
            a077325_res = res;
            a077325_n = n;
            return;
        }
        free(res);
        a077325_lim *= 2;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    if (n > a077325_n) {
        idx_t target = n;
        if (target < 64) target = 64;
        if (target < a077325_n * 2) target = a077325_n * 2;
        a077325_compute(target);
    }
    return (seq_t)a077325_res[n];
}
