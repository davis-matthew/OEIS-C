/*
 * A076245  Positions of records in A051547.
 * https://oeis.org/A076245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th m such that lcm{phi(1..m)} > lcm{phi(1..m-1)} (m=1
 *            counted as the first record); computed by a sieve tracking the
 *            maximal exponent of each prime in phi(m), doubling the limit as
 *            needed
 * Range:     u8 n <= 30, u16 n <= 1335, u32 n <= 13256, u64 n <= 13256,
 *            u128 n <= 13256, big any n
 * Validated: standard, pass, 72 terms through n = 9942 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static uint64_t *rec_list = NULL;
static uint64_t rec_cnt = 0;
static uint64_t rec_limit = 0;

static void rec_build(uint64_t L)
{
    uint32_t *spf = (uint32_t *)calloc((size_t)(L + 1), sizeof(uint32_t));
    uint8_t *maxe = (uint8_t *)calloc((size_t)(L + 1), sizeof(uint8_t));
    uint64_t *out = (uint64_t *)malloc((size_t)(L + 1) * sizeof(uint64_t));
    if (!spf || !maxe || !out) { free(spf); free(maxe); free(out); return; }
    for (uint64_t i = 2; i <= L; ++i) {
        if (spf[i] == 0) {
            for (uint64_t j = i; j <= L; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    uint64_t cnt = 0;
    out[cnt++] = 1;
    for (uint64_t m = 2; m <= L; ++m) {
        /* compute phi(m) */
        uint64_t x = m, ph = m;
        while (x > 1) {
            uint64_t p = spf[x];
            ph -= ph / p;
            while (x % p == 0) x /= p;
        }
        /* factor phi(m) */
        bool rec = false;
        uint64_t y = ph;
        while (y > 1) {
            uint64_t p = spf[y];
            int e = 0;
            while (y % p == 0) { y /= p; ++e; }
            if (e > maxe[p]) { maxe[p] = (uint8_t)e; rec = true; }
        }
        if (rec) out[cnt++] = m;
    }
    free(spf);
    free(maxe);
    free(rec_list);
    rec_list = out;
    rec_cnt = cnt;
    rec_limit = L;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (rec_limit == 0 || n > rec_cnt) {
        uint64_t L = rec_limit ? rec_limit : 2048;
        do {
            L = (rec_limit == 0) ? L : L * 2;
            rec_build(L);
            if (rec_limit != L) return 0;
            if (n <= rec_cnt) break;
            rec_limit = L;
            L = L * 2;
            rec_build(L);
            if (rec_limit != L) return 0;
        } while (n > rec_cnt && L < ((uint64_t)1 << 31));
    }
    if (n > rec_cnt) return 0;
    return (seq_t)rec_list[n - 1];
}
